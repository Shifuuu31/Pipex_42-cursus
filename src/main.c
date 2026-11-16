/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:15:57 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/07 11:02:59 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** Forks execution into child (running cmd1 via parent())
** and parent (running cmd2 via child()).
*/
void	execute(t_all pipex, char **av, char **env)
{
	int	pid;

	pid = fork();
	if (pid == 0)
		parent(pipex, av, env);
	child(pipex, av, env);
}

/*
** Entry point: validates arguments, opens file descriptors,
** creates the pipe, retrieves PATH, and launches execution.
*/
int	main(int ac, char **av, char **env)
{
	t_all	pipex;

	if (ac != 5)
		display_msg(ERR_INPUT);
	pipex.infile = open(av[1], O_RDONLY);
	if (pipex.infile < 0)
		display_msg(ERR_INFILE);
	pipex.outfile = open(av[ac - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (pipex.outfile < 0)
		display_msg(ERR_OUTFILE);
	if (pipe(pipex.pipe) < 0)
		display_msg(ERR_PIPE);
	pipex.path = find_path(&pipex, env);
	pipex.paths = ft_split(pipex.path, ':');
	execute(pipex, av, env);
	close_pipes(&pipex);
	free_parent(&pipex);
	return (0);
}
