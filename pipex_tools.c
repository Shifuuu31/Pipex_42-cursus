#include "pipex.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (0);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		str[j++] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
	{
		str[j++] = s2[i];
		i++;
	}
	str[j] = 0;
	return (str);
}

void free_parent(t_all *pipex) {
    char **path;

	path = pipex->paths;
    close(pipex->infile);
    close(pipex->outfile);
    while (*path) 
	{
        free(*path++);
    }
    free(pipex->paths);
}


void	free_child(t_all *pipex)
{
	char ** args;
	args = pipex->cmd_args;
	while (*args)
	{
		free(args++);
	}
	free(pipex->cmd_args);
	free(pipex->cmd);
}

