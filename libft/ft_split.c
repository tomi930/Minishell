#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*next_word(char const *s, char c, size_t *len)
{
	char	*word;

	while (*s == c)
		s++;
	*len = 0;
	while (s[*len] && s[*len] != c)
		(*len)++;
	word = malloc(*len + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, s, *len);
	word[*len] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		words;
	int		i;
	size_t	len;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	result = malloc(sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < words)
	{
		while (*s == c)
			s++;
		result[i] = next_word(s, c, &len);
		if (!result[i])
		{
			while (i-- > 0)
				free(result[i]);
			return (free(result), NULL);
		}
		s += len;
		i++;
	}
	result[words] = NULL;
	return (result);
}
