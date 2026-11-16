/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:16:25 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/07 10:59:59 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** Locates the PATH variable within the environment array.
** Returns a pointer to the colon-separated directory list.
*/
char	*find_path(t_all *pipex, char **env)
{
	int	i;
	int	path_index;

	i = 0;
	path_index = -1;
	while (env[i] != NULL)
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
		{
			path_index = i;
			break ;
		}
		i++;
	}
	if (path_index == -1)
	{
		perror(ERR_ENV);
		close_pipes(pipex);
		close(pipex->infile);
		close(pipex->outfile);
		exit(1);
	}
	return (env[path_index] + 5);
}

/*
** Resolves the binary executable path for a command by searching
** through candidate directories from PATH using access(path, X_OK).
*/
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
