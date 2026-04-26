/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_name_start(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

int	is_name_char(char c)
{
	return (is_name_start(c) || (c >= '0' && c <= '9'));
}

int	append_char(t_expand *exp, char c)
{
	char	*new_out;

	new_out = malloc(exp->len + 2);
	if (!new_out)
		return (0);
	if (exp->out)
	{
		ft_memcpy(new_out, exp->out, exp->len);
		free(exp->out);
	}
	new_out[exp->len] = c;
	new_out[exp->len + 1] = '\0';
	exp->out = new_out;
	exp->len++;
	return (1);
}

int	append_str(t_expand *exp, const char *s)
{
	size_t	slen;
	char	*new_out;

	if (!s)
		return (1);
	slen = ft_strlen(s);
	new_out = malloc(exp->len + slen + 1);
	if (!new_out)
		return (0);
	if (exp->out)
	{
		ft_memcpy(new_out, exp->out, exp->len);
		free(exp->out);
	}
	ft_memcpy(new_out + exp->len, s, slen);
	new_out[exp->len + slen] = '\0';
	exp->out = new_out;
	exp->len += slen;
	return (1);
}
