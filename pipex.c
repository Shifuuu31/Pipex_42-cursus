/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:16:25 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/06 21:16:26 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_pipes(t_all *pipex)
{
	close(pipex->pipe[0]);
	close(pipex->pipe[1]);
}

char *find_path(t_all *pipex, char **env)
{
    int i;
    int path_index;

	i = 0;
	path_index = -1;
    while (env[i] != NULL)
	{
        if (strncmp(env[i], "PATH=", 5) == 0)
		{
            path_index = i;
            break;
        }
        i++;
    }

    if (path_index == -1) {
        write(2, ERR_ENV, ft_strlen(ERR_ENV));
        close_pipes(pipex);
        close(pipex->infile);
        close(pipex->outfile);
        exit(1);
    }
    return env[path_index] + 5;
}

char	*command(char **paths, char *cmd)
{
	char	*path;
	char	*tmp;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		paths++;
	}
	return (NULL);
}

void	child_one(t_all pipex, char **av, char **env)
{
	char *tmp;
	tmp = av[2];
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
	if(*pipex.cmd  != '/')
		pipex.cmd = command(pipex.paths, pipex.cmd_args[0]);
    if (execve(pipex.cmd, pipex.cmd_args, env) == -1)
   {
	write(2, ERR_CMD, ft_strlen(ERR_CMD));
   	exit(1);
   }
}

void	child_two(t_all pipex, char **av, char **env)
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
	if(*pipex.cmd  != '/')
		pipex.cmd = command(pipex.paths, pipex.cmd_args[0]);
    if (execve(pipex.cmd, pipex.cmd_args, env) == -1)
    {
        write(2, ERR_CMD, ft_strlen(ERR_CMD));
        exit(1);
    }
}