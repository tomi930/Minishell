/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trus <trus@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/27 16:25:30 by trus             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quote(t_expand *exp, const char *token)
{
	if (!exp->quote && (token[exp->i] == '\'' || token[exp->i] == '"'))
	{
		exp->quote = token[exp->i];
		exp->i++;
		return (1);
	}
	if (exp->quote && token[exp->i] == exp->quote)
	{
		exp->quote = 0;
		exp->i++;
		return (1);
	}
	return (0);
}

char	*expand_token(const char *token, t_env *env)
{
	t_expand	exp;

	exp.out = NULL;
	exp.len = 0;
	exp.i = 0;
	exp.quote = 0;
	while (token[exp.i])
	{
		if (handle_quote(&exp, token))
			continue ;
		if (token[exp.i] == '$' && exp.quote != '\'')
		{
			if (!append_exp_env(&exp, token, env))
				return (free(exp.out), NULL);
		}
		else if (!append_char(&exp, token[exp.i++]))
			return (free(exp.out), NULL);
	}
	if (!exp.out)
		exp.out = ft_strdup("");
	return (exp.out);
}
