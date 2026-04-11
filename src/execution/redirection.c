#include "minishell.h"

static int	apply_redir_out(t_redir *r)
{
	int	flags;
	int	fd;

	if (r->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(r->target, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(r->target, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	apply_redir(t_redir *r)
{
	int	fd;

	if (r->type == REDIR_IN || r->type == REDIR_HEREDOC)
	{
		fd = open(r->target, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(r->target, 2);
			ft_putendl_fd(": No such file or directory", 2);
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		if (r->type == REDIR_HEREDOC)
			unlink(r->target);
		return (0);
	}
	return (apply_redir_out(r));
}

int	setup_redirections(t_cmd *cmd)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (apply_redir(r) == -1)
			return (-1);
		r = r->next;
	}
	return (0);
}
