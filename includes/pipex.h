/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:16:27 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/07 10:59:12 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define ERR_ENV "environment error !!\n"
# define ERR_INPUT "wrong number of arguments !!\n"
# define ERR_CMD "command not found !!\n"
# define ERR_INFILE "Infile error !!"
# define ERR_OUTFILE "Outfile error !!"
# define ERR_PIPE "Pipe error !!"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_all
{
	pid_t	pid_1;
	pid_t	pid_2;
	char	**cmd_args;
	char	*path;
	char	*cmd;
	char	**paths;
	int		infile;
	int		outfile;
	int		pipe[2];
}	t_all;

void	*ft_memcpy(void *dst, const void *src, size_t n);
void	parent(t_all pipex, char **av, char **env);
void	child(t_all pipex, char **av, char **env);
void	second_child(t_all pipex, char **av, char **env);
void	close_pipes(t_all *pipex);
void	free_child(t_all *pipex);
void	free_parent(t_all *pipex);
void	display_msg(char *msg);

size_t	ft_strlen(char *s);

int		ft_strncmp(const char *str1, const char *str2, size_t n);

char	*find_path(t_all *pipex, char **env);
char	*command(char **paths, char *cmd);
char	*ft_substr(char *s, size_t start, size_t len);
char	**ft_split(const char *s, char c);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strdup( char *s);

#endif