#include "minishell.h"

static int	is_name_start(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

static int	is_name_char(char c)
{
	return (is_name_start(c) || (c >= '0' && c <= '9'));
}

static int	append_char(char **out, size_t *len, char c)
{
	char	*new_out;

	new_out = malloc(*len + 2);
	if (!new_out)
		return (0);
	if (*out)
	{
		ft_memcpy(new_out, *out, *len);
		free(*out);
	}
	new_out[*len] = c;
	new_out[*len + 1] = '\0';
	*out = new_out;
	(*len)++;
	return (1);
}

static int	append_str(char **out, size_t *len, const char *s)
{
	size_t	slen;
	char	*new_out;

	if (!s)
		return (1);
	slen = ft_strlen(s);
	new_out = malloc(*len + slen + 1);
	if (!new_out)
		return (0);
	if (*out)
	{
		ft_memcpy(new_out, *out, *len);
		free(*out);
	}
	ft_memcpy(new_out + *len, s, slen);
	new_out[*len + slen] = '\0';
	*out = new_out;
	*len += slen;
	return (1);
}

static int	append_env(char **out, size_t *len, const char *token, int *i, t_env *env)
{
	int		start;
	char	*key;
	char	*value;
	char	*status;

	if (token[*i + 1] == '?')
	{
		*i += 2;
		status = ft_itoa(g_exit_status);
		if (!status)
			return (0);
		start = append_str(out, len, status);
		free(status);
		return (start);
	}
	if (!is_name_start(token[*i + 1]))
	{
		(*i)++;
		return (append_char(out, len, '$'));
	}
	start = ++(*i);
	while (token[*i] && is_name_char(token[*i]))
		(*i)++;
	key = ft_substr(token, start, *i - start);
	if (!key)
		return (0);
	value = get_env(key, env);
	free(key);
	return (append_str(out, len, value));
}

char	*expand_token(const char *token, t_env *env)
{
	char	*out;
	size_t	len;
	int		i;
	char	quote;

	out = NULL;
	len = 0;
	i = 0;
	quote = 0;
	while (token[i])
	{
		if (!quote && (token[i] == '\'' || token[i] == '"'))
			quote = token[i++];
		else if (quote && token[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if (token[i] == '$' && quote != '\'')
		{
			if (!append_env(&out, &len, token, &i, env))
				return (free(out), NULL);
		}
		else if (!append_char(&out, &len, token[i++]))
			return (free(out), NULL);
	}
	if (!out)
		out = ft_strdup("");
	return (out);
}
