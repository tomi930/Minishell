/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

int	token_is_op_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	add_token(char ***tokens, int *count, int *cap, char *token)
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
