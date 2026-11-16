/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:16:25 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/07 10:59:59 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** Closes both file descriptors of the kernel pipe buffer.
*/
void	close_pipes(t_all *pipex)
{
	close(pipex->pipe[0]);
	close(pipex->pipe[1]);
}

/*
** Child process worker (pid == 0):
** Redirects standard output to pipe[1] and standard input to infile,
** resolves cmd1 via PATH, and executes it via execve.
*/
void	parent(t_all pipex, char **av, char **env)
{
	dup2(pipex.pipe[1], 1);
	close(pipex.pipe[0]);
	dup2(pipex.infile, 0);
	pipex.cmd_args = ft_split(av[2], ' ');
	pipex.cmd = av[2];
	if (!pipex.cmd)
	{
		free_child(&pipex);
		write(2, ERR_CMD, ft_strlen(ERR_CMD));
		exit(1);
	}
	if (*pipex.cmd != '/')
		pipex.cmd = command(pipex.paths, pipex.cmd_args[0]);
	if (execve(pipex.cmd, pipex.cmd_args, env) == -1)
	{
		write(2, ERR_CMD, ft_strlen(ERR_CMD));
		exit(1);
	}
}

/*
** Parent process worker (pid > 0):
** Redirects standard input to pipe[0] and standard output to outfile,
** closes pipe[1] to trigger EOF on pipe closure, resolves cmd2 via PATH,
** and executes it via execve.
*/
void	child(t_all pipex, char **av, char **env)
{
	dup2(pipex.pipe[0], 0);
	close(pipex.pipe[1]);
	dup2(pipex.outfile, 1);
	pipex.cmd_args = ft_split(av[3], ' ');
	pipex.cmd = av[3];
	if (!pipex.cmd)
	{
		free_child(&pipex);
		write(2, ERR_CMD, ft_strlen(ERR_CMD));
		exit(1);
	}
	if (*pipex.cmd != '/')
		pipex.cmd = command(pipex.paths, pipex.cmd_args[0]);
	if (execve(pipex.cmd, pipex.cmd_args, env) == -1)
	{
		write(2, ERR_CMD, ft_strlen(ERR_CMD));
		exit(1);
	}
}
