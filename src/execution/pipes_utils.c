/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/27 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_exit(t_pipe_ctx *ctx, int code)
{
	rl_clear_history();
	free_env(*(ctx->env));
	free_cmd(ctx->head);
	exit(code);
}

void	pipe_setup_fds(t_cmd *cmd, t_pipe_ctx *ctx)
{
	if (ctx->prev_fd != -1)
		dup2(ctx->prev_fd, STDIN_FILENO);
	if (cmd->next)
	{
		close(ctx->pipefd[0]);
		dup2(ctx->pipefd[1], STDOUT_FILENO);
		close(ctx->pipefd[1]);
	}
	if (ctx->prev_fd != -1)
		close(ctx->prev_fd);
}
