/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_status(t_expand *exp)
{
	char	*status;
	int		ok;

	exp->i += 2;
	status = ft_itoa(g_exit_status);
	if (!status)
		return (0);
	ok = append_str(exp, status);
	free(status);
	return (ok);
}

int	append_exp_env(t_expand *exp, const char *token, t_env *env)
{
	int		start;
	char	*key;
	char	*value;

	if (token[exp->i + 1] == '?')
		return (append_status(exp));
	if (!is_name_start(token[exp->i + 1]))
	{
		exp->i++;
		return (append_char(exp, '$'));
	}
	exp->i++;
	start = exp->i;
	while (token[exp->i] && is_name_char(token[exp->i]))
		exp->i++;
	key = ft_substr(token, start, exp->i - start);
	if (!key)
		return (0);
	value = get_env(key, env);
	free(key);
	return (append_str(exp, value));
}
