/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrrokaj <hrrokaj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/26 00:00:00 by hrrokaj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_tmp_path(void)
{
	static int	seq;
	char		*pid;
	char		*id;
	char		*tmp;
	char		*path;

	pid = ft_itoa(getpid());
	if (!pid)
		return (NULL);
	id = ft_itoa(seq++);
	if (!id)
		return (free(pid), NULL);
	tmp = ft_strjoin("/tmp/minishell_heredoc_", pid);
	free(pid);
	if (!tmp)
		return (free(id), NULL);
	path = ft_strjoin(tmp, "_");
	free(tmp);
	if (!path)
		return (free(id), NULL);
	tmp = ft_strjoin(path, id);
	free(path);
	free(id);
	return (tmp);
}

static int	write_heredoc_line(int fd, char *line, int expand, t_env *env)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_token(line, env);
		if (!expanded)
			return (0);
		ft_putendl_fd(expanded, fd);
		free(expanded);
	}
	else
		ft_putendl_fd(line, fd);
	return (1);
}

char	*read_heredoc(char *delimiter, int expand, t_env *env)
{
	char	*path;
	char	*line;
	int		fd;

	path = heredoc_tmp_path();
	if (!path)
		return (NULL);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (free(path), NULL);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delimiter))
			break ;
		if (!write_heredoc_line(fd, line, expand, env))
			return (free(line), close(fd), unlink(path), free(path), NULL);
		free(line);
	}
	free(line);
	close(fd);
	return (path);
}
