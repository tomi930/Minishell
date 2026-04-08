#include "minishell.h"

static int	redirect_input(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->infile, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (cmd->is_heredoc)
		unlink(cmd->infile);
	return (0);
}

static int	redirect_output(t_cmd *cmd)
{
	int	fd;
	int	flags;

	if (cmd->append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(cmd->outfile, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->outfile, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	setup_redirections(t_cmd *cmd)
{
	if (cmd->infile && redirect_input(cmd) == -1)
		return (-1);
	if (cmd->outfile && redirect_output(cmd) == -1)
		return (-1);
	return (0);
}


//i'll rewrite it to cover redirection with t_redirs instead of infile/outfile