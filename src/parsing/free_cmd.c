/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redirs(t_redir *redir)
{
	t_redir	*next_redir;

	while (redir)
	{
		next_redir = redir->next;
		if (redir->type == REDIR_HEREDOC)
			unlink(redir->target);
		free(redir->target);
		free(redir);
		redir = next_redir;
	}
}

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free_redirs(cmd->redirs);
		free_args(cmd->args);
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		cmd = next;
	}
}
