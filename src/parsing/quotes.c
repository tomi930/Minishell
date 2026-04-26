/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_unclosed_quotes(const char *line)
{
	char	quote;

	quote = 0;
	while (*line)
	{
		if (!quote && (*line == '\'' || *line == '"'))
			quote = *line;
		else if (quote && *line == quote)
			quote = 0;
		line++;
	}
	return (quote != 0);
}

char	*strip_quotes(const char *token)
{
	char	*result;
	size_t	i;
	size_t	j;
	char	quote;

	result = malloc(ft_strlen(token) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (token[i])
	{
		if (!quote && (token[i] == '\'' || token[i] == '"'))
			quote = token[i];
		else if (quote && token[i] == quote)
			quote = 0;
		else
			result[j++] = token[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	token_has_quotes(const char *token)
{
	while (*token)
	{
		if (*token == '\'' || *token == '"')
			return (1);
		token++;
	}
	return (0);
}
