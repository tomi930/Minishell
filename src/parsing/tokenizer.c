/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	**init_tokens(int *i, int *count, int *cap)
{
	char	**tokens;

	*cap = 16;
	tokens = malloc(sizeof(char *) * (*cap));
	if (!tokens)
		return (NULL);
	*i = 0;
	*count = 0;
	tokens[0] = NULL;
	return (tokens);
}

char	**tokenize(char *line)
{
	char	**tokens;
	int		i;
	int		count;
	int		cap;
	char	*token;

	tokens = init_tokens(&i, &count, &cap);
	if (!tokens)
		return (NULL);
	while (line[i])
	{
		while (line[i] && token_is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (token_is_op_char(line[i]))
			token = read_operator(line, &i);
		else
			token = read_word(line, &i);
		if (!token || !add_token(&tokens, &count, &cap, token))
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
