/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:16:21 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/07 11:02:16 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** Releases heap memory allocated for PATH directories and closes file descriptors.
*/
void	free_parent(t_all *pipex)
{
	char	**paths;

	close(pipex->infile);
	close(pipex->outfile);
	paths = pipex->paths;
	while (*paths)
	{
		free(*paths++);
	}
	free(pipex->paths);
}

/*
** Releases heap memory allocated for command arguments array and command string.
*/
void	free_child(t_all *pipex)
{
	char	**args;

	args = pipex->cmd_args;
	while (*args)
	{
		free(args++);
	}
	free(pipex->cmd_args);
	free(pipex->cmd);
}

/*
** Error reporting helper: displays error string via perror and terminates process.
*/
void	display_msg(char *msg)
{
	perror(msg);
	exit(1);
}
