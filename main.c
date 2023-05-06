/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:15:57 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/06 21:17:34 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void exex (t_all pipex,char **av,char **env)
{
	int pid;
	pid = fork();
	if (pid == 0)
		child_one(pipex, av, env);
	child_two(pipex, av, env);
}
int	main(int ac, char **av, char **env)
{
	t_all	pipex;
	int pid;

	if (ac != 5)
		return (write(2, ERR_INPUT, ft_strlen(ERR_INPUT)));
	pipex.infile = open(av[1], O_RDONLY);
	if (pipex.infile < 0)
		{
			write(2, ERR_INFILE, ft_strlen(ERR_INFILE));
			exit (1);
		}
	pipex.outfile = open(av[ac - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (pipex.outfile < 0)
		{
			write(2, ERR_OUTFILE, ft_strlen(ERR_OUTFILE));
			exit (1);
		}
	if (pipe(pipex.pipe) < 0)
		{
			write(2, ERR_PIPE, ft_strlen(ERR_PIPE));
			exit (1);
		}
	pipex.path = find_path(&pipex, env);
	pipex.paths = ft_split(pipex.path, ':');
	pid = fork();

	if (pid == 0)
		child_one(pipex, av, env);
	child_two(pipex, av, env);
	//exex(pipex,av,env);
	close_pipes(&pipex);
	free_parent(&pipex);
	return (0);
}
