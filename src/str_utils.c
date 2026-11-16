/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:16:17 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/06 21:16:18 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** Measures and returns the length of null-terminated string 's'.
*/
size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

/*
** Copies 'n' bytes from memory area 'src' to memory area 'dst'.
*/
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (dst == src)
		return (dst);
	if (!dst && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		*(char *)(dst + i) = *(char *)(src + i);
		i++;
	}
	return (dst);
}

/*
** Allocates and returns a pointer to a newly duplicated string 's'.
*/
char	*ft_strdup( char *s)
{
	size_t	len;
	char	*ptr;

	len = ft_strlen(s) + 1;
	ptr = malloc(sizeof(char) * len);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s, len);
	return (ptr);
}

/*
** Allocates and returns the concatenation of 's1' followed by 's2'.
*/
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

/*
** Compares at most 'n' characters of strings 'str1' and 'str2'.
*/
int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	if (str1 == str2)
		return (0);
	while (i < n - 1 && str1 && str2 && str1[i] == str2[i])
	{
		i++;
		if (!str1[i] || !str2[i])
		{
			return ((unsigned char)str1[i] - (unsigned char)str2[i]);
		}
	}
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}
