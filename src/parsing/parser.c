/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_syntax(char **tokens)
{
	int	i;

	if (!tokens || !tokens[0])
		return (1);
	if (!ft_strcmp(tokens[0], "|"))
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "|"))
		{
			if (!tokens[i + 1] || !ft_strcmp(tokens[i + 1], "|"))
				return (0);
		}
		else if (parse_is_redir(tokens[i]))
		{
			if (!tokens[i + 1] || parse_is_op(tokens[i + 1]))
				return (0);
		}
		i++;
	}
	if (i > 0 && !ft_strcmp(tokens[i - 1], "|"))
		return (0);
	return (1);
}

static int	parse_token(char **tokens, int *i, t_cmd **cur, t_env *env)
{
	char	*arg;

	if (!ft_strcmp(tokens[*i], "|"))
	{
		(*cur)->next = parse_new_cmd();
		if (!(*cur)->next)
			return (0);
		*cur = (*cur)->next;
		(*i)++;
	}
	else if (parse_is_redir(tokens[*i]))
	{
		if (!set_redirection(*cur, tokens[*i], tokens[*i + 1], env))
			return (0);
		*i += 2;
	}
	else
	{
		arg = expand_token(tokens[(*i)++], env);
		if (!arg || !parse_append_arg(*cur, arg))
			return (free(arg), 0);
	}
	return (1);
}

static t_cmd	*build_commands(char **tokens, t_env *env)
{
	t_cmd	*head;
	t_cmd	*cur;
	int		i;

	head = parse_new_cmd();
	if (!head)
		return (NULL);
	cur = head;
	i = 0;
	while (tokens[i])
	{
		if (!parse_token(tokens, &i, &cur, env))
			return (free_cmd(head), NULL);
	}
	return (head);
}

t_cmd	*parse(char *line, t_env *env)
{
	char	**tokens;
	t_cmd	*cmd;

	if (!line || !*line)
		return (NULL);
	if (has_unclosed_quotes(line))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		return (NULL);
	}
	tokens = tokenize(line);
	if (!tokens || !validate_syntax(tokens))
	{
		free_tokens(tokens);
		ft_putendl_fd("minishell: syntax error", 2);
		return (NULL);
	}
	cmd = build_commands(tokens, env);
	free_tokens(tokens);
	return (cmd);
}
