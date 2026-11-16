# Deep Dive: Unix Pipes, File Descriptors & Process Redirection

> **Component:** Operating Systems & Systems Programming Concepts  
> **Target Audience:** Junior Developers, Systems Students, Code Reviewers  
> **Core Topics:** Inter-Process Communication (IPC), `pipe()`, `fork()`, `dup2()`, `execve()`, Kernel Buffers

---

## 1. Introduction to Unix Pipelines

In Unix philosophy, programs are designed to do one thing well and communicate via text streams. The command:

```bash
< infile cmd1 | cmd2 > outfile
```

links two isolated programs so the output of `cmd1` seamlessly becomes the input of `cmd2`, without writing intermediate data to disk.

In `Pipex`, this pipeline is constructed manually using low-level POSIX system calls.

---

## 2. Core System Calls in the Implementation

| System Call | Prototype | Implementation Role in Pipex |
| :--- | :--- | :--- |
| `open` | `int open(const char *path, int flags, mode_t mode)` | Opens `infile` (`O_RDONLY`) and creates/truncates `outfile` (`O_TRUNC \| O_CREAT \| O_RDWR, 0644`). |
| `pipe` | `int pipe(int pipefd[2])` | Allocates a unidirectional kernel data buffer and returns two new file descriptors (`pipefd[0]` for reading, `pipefd[1]` for writing). |
| `fork` | `pid_t fork(void)` | Clones the calling process, creating an exact duplicate child process with separate memory and inherited file descriptors. |
| `dup2` | `int dup2(int oldfd, int newfd)` | Atomically duplicates `oldfd` onto `newfd`. Used to rewire `STDIN_FILENO (0)` and `STDOUT_FILENO (1)`. |
| `close` | `int close(int fd)` | Releases file descriptors. Crucial for signaling End-of-File (`EOF`) across the pipe. |
| `execve` | `int execve(const char *pathname, char *const argv[], char *const envp[])` | Overwrites current process image with the specified executable binary. |
| `access` | `int access(const char *pathname, int mode)` | Tests whether candidate binary paths in `PATH` exist and have execution permissions (`X_OK`). |

---

## 3. How Pipes Work Internally

A pipe is a **unidirectional FIFO (First-In, First-Out) byte stream** maintained in operating system kernel memory.

```text
               +----------------------------------------+
               |              Linux Kernel              |
               |                                        |
Writer Process |   +---------+   64 KB Ring   +-----+   | Reader Process
(cmd1 stdout)  |-->| pipe[1] |-->  Buffer  -->|pipe[0]--> (cmd2 stdin)
               |   +---------+     Buffer     +-----+   |
               +----------------------------------------+
```

### Characteristics:
1. **Unidirectional Flow:** Data written to `pipe[1]` can only be read from `pipe[0]`.
2. **Kernel Buffer Limit:** On modern Linux, pipes have a default capacity of 65,536 bytes (64 KB). If the buffer fills up, writes block until the reader consumes data.
3. **No File System Inodes:** Pipes exist purely in memory and do not touch storage media.

---

## 4. File Descriptors & The `dup2()` System Call

Every Unix process starts with three default POSIX file descriptors:

- `0` (`STDIN_FILENO`): Standard Input
- `1` (`STDOUT_FILENO`): Standard Output
- `2` (`STDERR_FILENO`): Standard Error

When a program like `grep` or `wc` reads from standard input, it issues `read(0, buf, count)`. It is completely unaware whether descriptor `0` is a keyboard, a socket, a disk file, or a pipe.

### Redirection Mechanism

`dup2(oldfd, newfd)` replaces `newfd` with a copy of `oldfd`.

#### In the Child Process (`cmd1`):
```c
// Redirect output to pipe write end
dup2(pipex.pipe[1], 1);

// Close unused read end
close(pipex.pipe[0]);

// Redirect input to input file
dup2(pipex.infile, 0);
```

#### In the Parent Process (`cmd2`):
```c
// Redirect input to pipe read end
dup2(pipex.pipe[0], 0);

// Close unused write end (CRUCIAL!)
close(pipex.pipe[1]);

// Redirect output to destination file
dup2(pipex.outfile, 1);
```

---

## 5. The Critical Importance of Closing `pipe[1]`

One of the most frequent sources of deadlocks in pipeline programming is failing to close the write end of a pipe in the reader process.

### Why `close(pipex.pipe[1])` is Mandatory:

1. A reader process reading from a pipe (`read(0, ...)`) blocks until either:
   - Data is available in the pipe buffer.
   - **All open write file descriptors pointing to that pipe are closed**, at which point `read()` returns `0` (`EOF`).
2. When `fork()` is called, the child inherits duplicate copies of all open file descriptors. Both the parent and the child possess write descriptors pointing to `pipe[1]`.
3. If the parent (`cmd2`) fails to close its own copy of `pipe[1]`, the pipe **still has an open writer (itself!)**, even after the child process (`cmd1`) finishes and exits.
4. Consequently, `cmd2` would hang indefinitely waiting for EOF on its standard input.
5. In `Pipex`, `child()` executes `close(pipex.pipe[1])` before calling `execve()`, ensuring clean termination and EOF propagation.

---

## 6. Command Discovery & PATH Resolution

When a user runs `"ls -l"`, the executable binary is not in the working directory. It resides in system directories such as `/usr/bin/ls` or `/bin/ls`.

### Step-by-Step Resolution in `src/pipex.c`:

1. **Scan `envp`:** `find_path()` iterates through environment strings searching for the prefix `"PATH="`:
   ```c
   while (env[i] != NULL)
   {
       if (strncmp(env[i], "PATH=", 5) == 0)
           return (env[i] + 5);
       i++;
   }
   ```
2. **Tokenize PATH:** `ft_split(path, ':')` breaks the colon-delimited string into candidate directory paths:
   ```text
   ["/usr/local/bin", "/usr/bin", "/bin", "/usr/sbin", ...]
   ```
3. **Candidate Validation:** `command()` loops through each directory:
   - Constructs candidate: `dir + "/" + cmd` (e.g., `"/usr/bin/cat"`).
   - Tests executability: `access(candidate, X_OK)`.
   - Returns the first valid path found.
4. **Execution:** The resolved path is passed to `execve()`, along with the arguments array and the environment pointer.
