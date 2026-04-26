/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_is_op(const char *t)
{
	return (!ft_strcmp(t, "|") || !ft_strcmp(t, "<") || !ft_strcmp(t, ">")
		|| !ft_strcmp(t, "<<") || !ft_strcmp(t, ">>"));
}

int	parse_is_redir(const char *t)
{
	return (!ft_strcmp(t, "<") || !ft_strcmp(t, ">")
		|| !ft_strcmp(t, "<<") || !ft_strcmp(t, ">>"));
}

t_cmd	*parse_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->is_heredoc = 0;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	parse_append_arg(t_cmd *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		count;

	count = 0;
	if (cmd->args)
		while (cmd->args[count])
			count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = arg;
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (1);
}
