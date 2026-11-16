/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadane <asaadane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:16:13 by asaadane          #+#    #+#             */
/*   Updated: 2023/05/06 21:19:33 by asaadane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** Extracts and returns a newly allocated substring from string 's'
** starting at index 'start' and of maximum length 'len'.
*/
char	*ft_substr(char *s, size_t start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (s[i])
	{
		if (i >= start && j < len)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
	return (str);
}

/* -------------------------------------------------------------------------- */
/*                               Static Helpers                               */
/* -------------------------------------------------------------------------- */

/*
** Helper: deallocates all split entries upon allocation failure.
*/
static void	*ft_free(int j, char **p)
{
	while (j > 0)
	{
		free(p[j]);
		j--;
	}
	free(p);
	return (NULL);
}

/*
** Helper: measures length of next word delimited by character 'c'.
*/
static	int	word_len(char *str, char c, int i)
{
	int	j;

	j = 0;
	while (str[i] != c && str[i])
	{
		j++;
		i++;
	}
	return (j);
}

/*
** Helper: counts total delimiter-separated words in string 'str'.
*/
static	size_t	word_counter(char const *str, char ch)
{
	size_t	i;
	size_t	counter;

	i = 0;
	counter = 0;
	while (str[i] != '\0')
	{
		while (str[i] == ch)
			i++;
		if (str[i])
			counter++;
		while (str[i] != ch && str[i])
			i++;
	}
	return (counter);
}

/* -------------------------------------------------------------------------- */
/*                               Public Engine                                */
/* -------------------------------------------------------------------------- */

/*
** Splits string 's' using delimiter character 'c' into a null-terminated
** array of newly allocated strings.
*/
char	**ft_split(const char *s, char c)
{
	int		i;
	int		j;
	int		wc;
	char	**split;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	wc = word_counter((char *)s, c);
	split = (char **)malloc((wc + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	while (j < wc && s[i])
	{
		while (s[i] == c && s[i])
				i++;
		split[j] = ft_substr((char *)s, i, word_len((char *)s, c, i));
		if (!split[j])
			return (ft_free(j, split));
		j++;
		i = i + word_len((char *)s, c, i);
	}
		split[j] = NULL;
	return (split);
}
