#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

typedef struct s_cmd
{
	char	*cmd;
	char	**path;
	char	*args[100];
}	t_cmd;

size_t      ft_strlen( char *s);
char        **ft_split(const char *s, char c);
int         ft_strncmp(const char *str1, const char *str2, size_t n);
char        *path_finder(char *path);
char	*ft_strjoin(char *s1, char *s2);
#endif