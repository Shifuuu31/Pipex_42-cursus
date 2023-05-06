#include "pipex.h"

int	main(int ac, char **av, char **env)
{
	t_all	pipex;

	// for(int i = 0; i < ac; i++) {
    //   printf("av[%d]: %s\n", i, av[i]);
//    }

	if (ac != 5)
		return (write(2, ERR_INPUT, ft_strlen(ERR_INPUT)));
	pipex.infile = open(av[1], O_RDONLY);
	if (pipex.infile < 0)
		{
			perror(ERR_INFILE);
			exit (1);
		}
	pipex.outfile = open(av[ac - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (pipex.outfile < 0)
		{
			perror(ERR_OUTFILE);
			exit (1);
		}
	if (pipe(pipex.pipe) < 0)
		{
			perror(ERR_PIPE);
			exit (1);
		}
	pipex.path = find_path(&pipex, env);
	pipex.paths = ft_split(pipex.path, ':');
	pipex.pid_1 = fork();
	if (pipex.pid_1 == 0)
		child_one(pipex, av, env);
	pipex.pid_2 = fork();
	if (pipex.pid_2 == 0)
		child_two(pipex, av, env);
	close_pipes(&pipex);
	free_parent(&pipex);
	return (0);
}
