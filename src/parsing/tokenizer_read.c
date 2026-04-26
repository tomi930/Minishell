/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_operator(char *line, int *i)
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

char	*read_word(char *line, int *i)
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
		else if (!quote
			&& (token_is_space(line[*i]) || token_is_op_char(line[*i])))
			break ;
		(*i)++;
	}
	return (ft_substr(line, start, *i - start));
}
