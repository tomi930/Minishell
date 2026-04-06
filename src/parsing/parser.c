#include "minishell.h"

static int	is_op(const char *t)
{
	return (!ft_strcmp(t, "|") || !ft_strcmp(t, "<") || !ft_strcmp(t, ">")
		|| !ft_strcmp(t, "<<") || !ft_strcmp(t, ">>"));
}

static int	is_redir(const char *t)
{
	return (!ft_strcmp(t, "<") || !ft_strcmp(t, ">")
		|| !ft_strcmp(t, "<<") || !ft_strcmp(t, ">>"));
}

static t_cmd	*new_cmd(void)
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

static int	append_arg(t_cmd *cmd, char *arg)
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

static int	token_has_quotes(const char *token)
{
	while (*token)
	{
		if (*token == '\'' || *token == '"')
			return (1);
		token++;
	}
	return (0);
}

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

static char	*read_heredoc(char *delimiter, int expand, t_env *env)
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

static int	set_redirection(t_cmd *cmd, char *op, char *target, t_env *env)
{
	char			*value;
	t_redir_type	type;

	if (!ft_strcmp(op, "<<"))
	{
		value = strip_quotes(target);
		if (!value)
			return (0);
		target = read_heredoc(value, !token_has_quotes(target), env);
		free(value);
		if (!target)
			return (0);
		value = target;
		type = REDIR_HEREDOC;
	}
	else
	{
		value = expand_token(target, env);
		if (!ft_strcmp(op, "<"))
			type = REDIR_IN;
		else if (!ft_strcmp(op, ">"))
			type = REDIR_OUT;
		else
			type = REDIR_APPEND;
	}
	if (!value)
		return (0);
	if (!append_redirection(cmd, type, value))
		return (0);
	if (type == REDIR_IN || type == REDIR_HEREDOC)
	{
		free(cmd->infile);
		cmd->infile = ft_strdup(value);
		cmd->is_heredoc = (type == REDIR_HEREDOC);
		if (!cmd->infile)
			return (0);
	}
	else
	{
		free(cmd->outfile);
		cmd->outfile = ft_strdup(value);
		cmd->append = (type == REDIR_APPEND);
		if (!cmd->outfile)
			return (0);
	}
	return (1);
}

static int	validate_syntax(char **tokens)
{
	int	i;

	if (!tokens || !tokens[0])
		return (1);
	if (!ft_strcmp(tokens[0], "|"))
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "|"))
		{
			if (!tokens[i + 1] || !ft_strcmp(tokens[i + 1], "|"))
				return (0);
		}
		else if (is_redir(tokens[i]))
		{
			if (!tokens[i + 1] || is_op(tokens[i + 1]))
				return (0);
		}
		i++;
	}
	if (i > 0 && !ft_strcmp(tokens[i - 1], "|"))
		return (0);
	return (1);
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*next;
	t_redir	*redir;
	t_redir	*next_redir;
	int		i;

	while (cmd)
	{
		next = cmd->next;
		redir = cmd->redirs;
		while (redir)
		{
			next_redir = redir->next;
			if (redir->type == REDIR_HEREDOC)
				unlink(redir->target);
			free(redir->target);
			free(redir);
			redir = next_redir;
		}
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		cmd = next;
	}
}

t_cmd	*parse(char *line, t_env *env)
{
	char	**tokens;
	t_cmd	*head;
	t_cmd	*cur;
	int		i;
	char	*arg;

	if (!line || !*line)
		return (NULL);
	if (has_unclosed_quotes(line))
		return (ft_putendl_fd("minishell: syntax error: unclosed quote", 2), NULL);
	tokens = tokenize(line);
	if (!tokens || !validate_syntax(tokens))
		return (free_tokens(tokens), ft_putendl_fd("minishell: syntax error", 2), NULL);
	head = new_cmd();
	if (!head)
		return (free_tokens(tokens), NULL);
	cur = head;
	i = 0;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "|"))
		{
			cur->next = new_cmd();
			if (!cur->next)
				return (free_tokens(tokens), free_cmd(head), NULL);
			cur = cur->next;
			i++;
		}
		else if (is_redir(tokens[i]))
		{
			if (!set_redirection(cur, tokens[i], tokens[i + 1], env))
				return (free_tokens(tokens), free_cmd(head), NULL);
			i += 2;
		}
		else
		{
			arg = expand_token(tokens[i++], env);
			if (!arg || !append_arg(cur, arg))
				return (free(arg), free_tokens(tokens), free_cmd(head), NULL);
		}
	}
	return (free_tokens(tokens), head);
}
