# Project Technical Audit: Pipex (42 Cursus)

> **Document Type:** Technical Audit & Codebase Verification Report  
> **Target Repository:** `Pipex_42-cursus`  
> **Original Author:** Aymane Saadane (`asaadane@student.1337.ma` / `student.42.fr`)  
> **Evaluation Outcome:** 100% Mandatory Part Validated (Intra 42)  
> **Audit Date:** November 2026  
> **Auditor Constraint:** Non-destructive audit. Source code and behavior preserved verbatim.

---

## 1. Executive Summary

This audit provides a comprehensive technical evaluation of the `Pipex_42-cursus` repository. The project reproduces the behavior of the Unix shell pipeline construct:

```bash
< infile cmd1 | cmd2 > outfile
```

Executed via the binary interface:

```bash
./pipex infile "cmd1" "cmd2" outfile
```

The repository was submitted and passed peer evaluation with a perfect score of **100/100** on the mandatory part. This audit reviews the implementation architecture, verifies system call usage, evaluates file descriptor handling, catalogs all functions, identifies architectural nuances and edge-case limitations, and establishes the empirical baseline for documentation and portfolio presentation.

---

## 2. File Responsibilities & Repository Structure

| File | Lines | Primary Responsibility |
| :--- | :--- | :--- |
| `src/main.c` | 45 | CLI entry point, argument validation (`argc == 5`), file opening, pipe creation, PATH discovery, process dispatching (`execute()`). |
| `src/pipex.c` | 108 | Pipeline execution logic: `find_path()`, `command()` resolution, `parent()` (executes `cmd1`), and `child()` (executes `cmd2`). |
| `includes/pipex.h` | 63 | Header declarations, macro error strings, header inclusions (`<unistd.h>`, `<fcntl.h>`, `<sys/wait.h>`), and definition of `t_all` struct. |
| `src/pipex_tools.c` | 74 | Memory deallocation routines (`free_parent`, `free_child`), string concatenation (`ft_strjoin`), and error exit helper (`display_msg`). |
| `src/pipex_tools_2.c` | 113 | String tokenization utilities: `ft_split`, substring extraction (`ft_substr`), word counting, and split-cleanup helpers. |
| `src/pipex_tools_3.c` | 73 | String primitives: `ft_strlen`, memory copy (`ft_memcpy`), string duplication (`ft_strdup`), and string comparison (`ft_strncmp`). |
| `Makefile` | 41 | Compilation management with `-Wall -Wextra -Werror`, targets `all`, `$(NAME)`, `clean`, `fclean`, `re`, and banner rule `shifuuu`. |
| `en.subject.pdf` | 12 pp | Official 42 project specification document (Version 5.0). |
| `pipex-100.png` | Image | Verification artifact demonstrating 100% grade on 42 Intra. |

---

## 3. Function Inventory

The table below catalogs every function implemented or declared in the project:

| Function | File | Scope | Prototype | Description & Lifecycle Impact |
| :--- | :--- | :--- | :--- | :--- |
| `main` | `src/main.c` | Global | `int main(int ac, char **av, char **env)` | Validates argument count, opens files, creates pipe, splits PATH, triggers execution. |
| `execute` | `src/main.c` | Global | `void execute(t_all pipex, char **av, char **env)` | Calls `fork()`. Dispatches `parent()` for child (`pid == 0`) and `child()` for parent (`pid > 0`). |
| `close_pipes` | `src/pipex.c` | Global | `void close_pipes(t_all *pipex)` | Closes both `pipe[0]` and `pipe[1]` file descriptors. |
| `find_path` | `src/pipex.c` | Global | `char *find_path(t_all *pipex, char **env)` | Scans `envp` for `PATH=`. Returns pointer past the prefix or exits on error. |
| `command` | `src/pipex.c` | Global | `char *command(char **paths, char *cmd)` | Iterates over PATH directory candidates; verifies executability with `access(path, X_OK)`. Returns allocated string. |
| `parent` | `src/pipex.c` | Global | `void parent(t_all pipex, char **av, char **env)` | **Executes in Child (`pid == 0`)**: Redirects `infile` -> STDIN, `pipe[1]` -> STDOUT, closes `pipe[0]`, runs `execve(cmd1)`. |
| `child` | `src/pipex.c` | Global | `void child(t_all pipex, char **av, char **env)` | **Executes in Parent (`pid > 0`)**: Redirects `pipe[0]` -> STDIN, `outfile` -> STDOUT, closes `pipe[1]`, runs `execve(cmd2)`. |
| `second_child` | `includes/pipex.h` | Prototype | `void second_child(t_all pipex, char **av, char **env);` | **Unused Prototype**: Declared in header, but not implemented in any `.c` file. |
| `ft_strjoin` | `src/pipex_tools.c` | Global | `char *ft_strjoin(char *s1, char *s2)` | Allocates and returns concatenation of `s1` and `s2`. |
| `free_parent` | `src/pipex_tools.c` | Global | `void free_parent(t_all *pipex)` | Closes file descriptors and frees the split `paths` array. |
| `free_child` | `src/pipex_tools.c` | Global | `void free_child(t_all *pipex)` | Frees `cmd_args` array and `cmd` string (contains pointer-increment bug). |
| `display_msg` | `src/pipex_tools.c` | Global | `void display_msg(char *msg)` | Calls `perror(msg)` and immediately terminates with `exit(1)`. |
| `ft_substr` | `src/pipex_tools_2.c`| Global | `char *ft_substr(char *s, size_t start, size_t len)` | Allocates substring from `s` starting at index `start`. |
| `ft_free` | `src/pipex_tools_2.c`| Static | `static void *ft_free(int j, char **p)` | Helper that frees elements of 2D array on allocation failure. |
| `word_len` | `src/pipex_tools_2.c`| Static | `static int word_len(char *str, char c, int i)` | Measures character count of next token delimited by `c`. |
| `word_counter`| `src/pipex_tools_2.c`| Static | `static size_t word_counter(char const *str, char ch)` | Counts delimiter-separated tokens in `str`. |
| `ft_split` | `src/pipex_tools_2.c`| Global | `char **ft_split(const char *s, char c)` | Splits string `s` by delimiter `c` into null-terminated array of strings. |
| `ft_strlen` | `src/pipex_tools_3.c`| Global | `size_t ft_strlen(char *s)` | Calculates length of string `s`. |
| `ft_memcpy` | `src/pipex_tools_3.c`| Global | `void *ft_memcpy(void *dst, const void *src, size_t n)` | Copies `n` bytes from `src` memory area to `dst`. |
| `ft_strdup` | `src/pipex_tools_3.c`| Global | `char *ft_strdup(char *s)` | Allocates duplicate of string `s`. |
| `ft_strncmp` | `src/pipex_tools_3.c`| Global | `int ft_strncmp(const char *str1, const char *str2, size_t n)` | Compares up to `n` characters of strings `str1` and `str2`. |

---

## 4. Architectural Analysis

### 4.1 Process Model & Execution Flow

```text
                           [main()]
                              |
       +----------------------+----------------------+
       | Validate argc == 5                          |
       | Open infile (O_RDONLY)                      |
       | Open outfile (O_TRUNC | O_CREAT | O_RDWR)   |
       | Create pipe(pipe[2])                        |
       | Resolve PATH & split directories            |
       +---------------------------------------------+
                              |
                     [execute(): fork()]
                              |
            +-----------------+-----------------+
            |                                   |
      (pid == 0)                          (pid > 0)
    [Child Process]                    [Parent Process]
  Runs parent() function:             Runs child() function:
  - dup2(infile, STDIN)               - dup2(pipe[0], STDIN)
  - dup2(pipe[1], STDOUT)             - dup2(outfile, STDOUT)
  - close(pipe[0])                    - close(pipe[1]) (sends EOF)
  - ft_split(av[2], ' ')              - ft_split(av[3], ' ')
  - Path lookup via access()          - Path lookup via access()
  - execve(cmd1)                      - execve(cmd2) [Replaces parent]
```

### 4.2 Key Architectural Characteristics

1. **Single Fork Asymmetric Architecture:**
   The conventional 42 implementation pattern forks twice (creating two child processes) while the main parent process closes pipe descriptors and waits for both children with `waitpid()`. In this repository, only **one `fork()`** is invoked. The child process executes `cmd1`, while the parent process replaces its own image with `cmd2` via `execve()`.

2. **Inverted Function Nomenclature:**
   The function executing the first command in the child process (`pid == 0`) is named `parent()`. Conversely, the function executing the second command in the original parent process (`pid > 0`) is named `child()`.

3. **Pipe Closure and EOF Signaling:**
   In `child()` (parent process executing `cmd2`), `close(pipex.pipe[1])` is executed before `execve()`. This ensures that no active write descriptors remain open in the reader's process space. Once `cmd1` finishes writing, the reader receives `EOF` cleanly, preventing pipeline hangs.

4. **Dead Code in `main.c`:**
   Lines 42–44 of `main.c` contain:
   ```c
   execute(pipex, av, env);
   close_pipes(&pipex);
   free_parent(&pipex);
   return (0);
   ```
   Because `execute()` dispatches both branches directly into `execve()` (which replaces the process image upon success or calls `exit(1)` upon failure), the cleanup calls following `execute()` are physically unreachable during normal operation.

---

## 5. Supported Behaviors

1. **Standard Two-Stage Pipeline:**
   Full parity with `< infile cmd1 | cmd2 > outfile` for commands resolvable via standard `PATH`.
2. **Flag and Option Parsing:**
   Supports commands with parameters when separated by standard whitespace (e.g., `"grep -E 'pattern'"` without nested quotes, `"sort -r"`, `"head -n 5"`).
3. **Truncated File Output:**
   Opens output destination with `O_TRUNC | O_CREAT | O_RDWR` with mode `0644`, correctly resetting target files.
4. **Environment Fallback:**
   Correctly identifies absence of `PATH` in `envp` and terminates with `environment error !!`.

---

## 6. Verified Limitations & Edge Cases

| Area | Observed Behavior | Technical Cause |
| :--- | :--- | :--- |
| **Absolute Paths with Arguments** | Fails with `command not found !!` when given `"/usr/bin/grep apple"`. | `if (*pipex.cmd != '/')` only evaluates the first character of the full argument string `av[2]`. When true, `pipex.cmd` is never resolved to `pipex.cmd_args[0]`, passing the whole string with spaces to `execve()`. |
| **Quoted Arguments / Subshells** | Quotes within command strings are split naively by space. | `ft_split(av[x], ' ')` does not parse single/double quote boundaries or backslash escapes. |
| **Process Synchronization** | Exit code of `./pipex` is solely determined by `cmd2`. | No `waitpid()` is called. If `cmd1` fails, `cmd2` may still succeed and exit `0`. `cmd1` is adopted by `init`. |
| **`free_child` Pointer Bug** | `free(args++)` attempts to free pointer to array element rather than pointed string. | `args++` yields `char **` rather than `*args` (`char *`). However, this function is only referenced in an unreachable error check (`if (!pipex.cmd)`). |
| **Makefile Terminal Dependency** | `make` fails with Error 1 in non-interactive / dumb terminals. | Rule `shifuuu` executes `clear` without `-` error suppression. Requires `TERM=xterm` or direct invocation of `make pipex`. |
| **Unused Structure Fields** | `pid_1` and `pid_2` defined in `struct s_all` are unused. | `execute()` declares a local `int pid;` instead of utilizing the struct fields. |

---

## 7. Build Instructions & Reproduction

### Standard Compilation
```bash
# In standard terminal:
make

# In headless/CI environments where TERM is unset:
TERM=xterm make
# or:
make pipex
```

### Clean Targets
```bash
make clean   # Removes object files (*.o)
make fclean  # Removes object files and pipex binary
make re      # Performs fclean and rebuilds
```

---

## 8. Testing Observations Summary

| Case ID | Command Executed | Expected Shell Result | Pipex Result | Status |
| :--- | :--- | :--- | :--- | :--- |
| `TC-01` | `./pipex in "grep Line" "wc -l" out` | Identical to `< in grep Line \| wc -l > out` | Identical byte-for-byte output | **PASS** |
| `TC-02` | `./pipex in "cat" "head -n 2" out` | Identical to `< in cat \| head -n 2 > out` | Identical byte-for-byte output | **PASS** |
| `TC-03` | `./pipex in "tr a-z A-Z" "sort -r" out` | Identical to `< in tr a-z A-Z \| sort -r > out` | Identical byte-for-byte output | **PASS** |
| `TC-04` | `./pipex in "/usr/bin/cat" "/usr/bin/wc" out` | Execution of absolute paths without parameters | Executed cleanly, exit code `0` | **PASS** |
| `TC-05` | `./pipex in "/usr/bin/grep Line" "wc -l" out` | Execution of absolute path with parameter | `command not found !!`, exit code `0` | **KNOWN LIMITATION** |
| `TC-06` | `./pipex nonexistent "cat" "wc -l" out` | Stderr warning, immediate termination | `Infile error !!: No such file or directory`, exit `1` | **PASS** |
| `TC-07` | `./pipex in "invalidcmd" "wc -l" out` | Stderr warning from cmd1 | `command not found !!`, exit code `0` (cmd2 succeeded) | **PASS** |
| `TC-08` | `./pipex in "grep Line" "invalidcmd" out` | Stderr warning from cmd2 | `command not found !!`, exit code `1` | **PASS** |
| `TC-09` | `./pipex in "cat" "wc -l"` (3 args) | Argument error message | `wrong number of arguments !!: Success`, exit `1` | **PASS** |

---

## 9. Documentation Gap Analysis

Prior to this project transformation:
- `README.md` contained solely `# PipeX` without installation instructions, conceptual background, or usage examples.
- No architecture documentation existed explaining the process model or file descriptor redirection.
- No explanation existed regarding how the program replicates Unix pipelines.
- No testing guide or verification artifact catalog was available.

---

## 10. Recommended Improvements (For Reference Only)

While the implementation remains locked pursuant to strict project constraints, future extensions (or bonus implementations) could address:
1. **Dual-Fork Model:** Refactoring `execute()` to fork two child processes and calling `waitpid()` for both to ensure accurate exit status propagation.
2. **Quote-Aware Parser:** Replacing naive whitespace `ft_split` with a quote-aware lexer to allow `awk '{print $1}'` and `sed 's/ /_/g'`.
3. **Absolute Path Tokenization:** Setting `cmd` to `cmd_args[0]` regardless of whether it starts with `'/'`.
4. **Makefile Resilience:** Adding `@-clear` to `shifuuu` rule in Makefile to prevent build failures in dumb terminal contexts.
