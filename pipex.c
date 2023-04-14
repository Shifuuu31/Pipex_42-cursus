#include "pipex.h"


void	cmd_1(int *fd, t_cmd *t, char **envp)
{
	int		i;
	char	*cmd;
    int infile = open("infile", O_RDONLY);

	i = 0;
	if (dup2(infile, STDIN_FILENO) < 0)
		return (perror("cmd_1"));
	while (t->path[i])
	{
		cmd = ft_strjoin(t->path[i], t->cmd);
		if (!cmd || !ft_strlen(cmd))
			break;
		if (execve(cmd, t->args, envp) != -1)
		{
			free(cmd);
			exit(EXIT_SUCCESS);
		}
		free(cmd);
        i++;
	}
	exit(EXIT_FAILURE);
}

void	pipex(int f1, int f2, char **ag, char **envp)
{
	t_cmd	cmd1;
	t_cmd	cmd2;

	init_cmd(&cmd1, f1);
	init_cmd(&cmd2, f2);
	if (!get_cmd(envp, &cmd1, ag[2]) || !get_cmd(envp, &cmd2, ag[3]))
		return (free_all(&cmd1, &cmd2));
	if (!check_cmd(&cmd1) || !check_cmd(&cmd2))
		return (free_all(&cmd1, &cmd2));
	exec_cmd(&cmd1, &cmd2, envp);
	free_struct(&cmd1);
	free_struct(&cmd2);
}