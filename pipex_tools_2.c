#include "pipex.h"

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

size_t	ft_strlen( char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

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
