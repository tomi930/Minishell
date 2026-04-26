/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parse.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_redirection(t_cmd *cmd, t_redir_type type, char *target)
{
	t_redir	*redir;
	t_redir	*last;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (free(target), 0);
	redir->type = type;
	redir->target = target;
	redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = redir;
	}
	return (1);
}

static char	*get_redir_value(char *op, char *target, t_env *env)
{
	char	*value;

	if (ft_strcmp(op, "<<"))
		return (expand_token(target, env));
	value = strip_quotes(target);
	if (!value)
		return (NULL);
	target = read_heredoc(value, !token_has_quotes(target), env);
	free(value);
	return (target);
}

static t_redir_type	get_redir_type(char *op)
{
	if (!ft_strcmp(op, "<<"))
		return (REDIR_HEREDOC);
	if (!ft_strcmp(op, "<"))
		return (REDIR_IN);
	if (!ft_strcmp(op, ">"))
		return (REDIR_OUT);
	return (REDIR_APPEND);
}

static int	update_cmd_redirection(t_cmd *cmd, char *value, t_redir_type type)
{
	if (type == REDIR_IN || type == REDIR_HEREDOC)
	{
		free(cmd->infile);
		cmd->infile = ft_strdup(value);
		cmd->is_heredoc = (type == REDIR_HEREDOC);
		return (cmd->infile != NULL);
	}
	free(cmd->outfile);
	cmd->outfile = ft_strdup(value);
	cmd->append = (type == REDIR_APPEND);
	return (cmd->outfile != NULL);
}

int	set_redirection(t_cmd *cmd, char *op, char *target, t_env *env)
{
	char			*value;
	t_redir_type	type;

	value = get_redir_value(op, target, env);
	if (!value)
		return (0);
	type = get_redir_type(op);
	if (!append_redirection(cmd, type, value))
		return (0);
	if (!update_cmd_redirection(cmd, value, type))
		return (0);
	return (1);
}
