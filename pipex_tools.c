#include "pipex.h"
#include <limits.h>
#include <stdlib.h>


char *path_finder(char *path)
{
    extern char** environ;
    char** env;

    env = environ;
    while(*env != NULL)
    {
        if (!strncmp(*env, "PATH=", 5))
        {
            strncpy(path, *env + 5, 123);
            path[122] = '\0';
        }
        env++;
    }
    return (path);
}

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