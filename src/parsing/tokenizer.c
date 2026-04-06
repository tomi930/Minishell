#include "minishell.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

static int	is_op_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	add_token(char ***tokens, int *count, int *cap, char *token)
{
	char	**new_tokens;
	int		i;

	if (*count + 1 >= *cap)
	{
		*cap *= 2;
		new_tokens = malloc(sizeof(char *) * (*cap));
		if (!new_tokens)
			return (free(token), 0);
		i = 0;
		while (i < *count)
		{
			new_tokens[i] = (*tokens)[i];
			i++;
		}
		free(*tokens);
		*tokens = new_tokens;
	}
	(*tokens)[(*count)++] = token;
	(*tokens)[*count] = NULL;
	return (1);
}

static char	*read_operator(char *line, int *i)
{
	char	*token;

	if ((line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '>' && line[*i + 1] == '>'))
	{
		token = ft_substr(line, *i, 2);
		*i += 2;
	}
	else
	{
		token = ft_substr(line, *i, 1);
		(*i)++;
	}
	return (token);
}

static char	*read_word(char *line, int *i)
{
	int		start;
	char	quote;

	start = *i;
	quote = 0;
	while (line[*i])
	{
		if (!quote && (line[*i] == '\'' || line[*i] == '"'))
			quote = line[*i];
		else if (quote && line[*i] == quote)
			quote = 0;
		else if (!quote && (is_space(line[*i]) || is_op_char(line[*i])))
			break ;
		(*i)++;
	}
	return (ft_substr(line, start, *i - start));
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

char	**tokenize(char *line)
{
	char	**tokens;
	int		i;
	int		count;
	int		cap;
	char	*token;

	cap = 16;
	tokens = malloc(sizeof(char *) * cap);
	if (!tokens)
		return (NULL);
	i = 0;
	count = 0;
	tokens[0] = NULL;
	while (line[i])
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (is_op_char(line[i]))
			token = read_operator(line, &i);
		else
			token = read_word(line, &i);
		if (!token || !add_token(&tokens, &count, &cap, token))
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
