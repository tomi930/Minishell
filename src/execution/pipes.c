#include "minishell.h"

static void	pipe_exec(t_cmd *cmd, t_pipe_ctx *ctx)
{
	char	**envp;
	char	*path;

	path = find_path(cmd->args[0], *(ctx->env));
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		rl_clear_history();
		free_env(*(ctx->env));
		free_cmd(ctx->head);
		exit(127);
	}
	envp = env_to_array(*(ctx->env));
	free_env(*(ctx->env));
	execve(path, cmd->args, envp);
	free(path);
	free_envp(envp);
	rl_clear_history();
	free_cmd(ctx->head);
	exit(1);
}

static void	pipe_child(t_cmd *cmd, t_pipe_ctx *ctx)
{
	int	ret;

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
	setup_redirections(cmd);
	if (is_builtin(cmd->args[0]))
	{
		ret = exec_builtin(cmd, ctx->env);
		rl_clear_history();
		free_env(*(ctx->env));
		free_cmd(ctx->head);
		exit(ret);
	}
	pipe_exec(cmd, ctx);
}

static void	update_fds(t_pipe_ctx *ctx, t_cmd *cmd)
{
	if (ctx->prev_fd != -1)
		close(ctx->prev_fd);
	if (cmd->next)
	{
		close(ctx->pipefd[1]);
		ctx->prev_fd = ctx->pipefd[0];
	}
}

static void	wait_all(pid_t last_pid)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
			g_exit_status = WEXITSTATUS(status);
		pid = waitpid(-1, &status, 0);
	}
}

void	exec_pipeline(t_cmd *cmd, t_env **env)
{
	t_pipe_ctx	ctx;
	pid_t		last_pid;
	pid_t		pid;

	ctx.head = cmd;
	ctx.env = env;
	ctx.prev_fd = -1;
	last_pid = -1;
	while (cmd)
	{
		if (cmd->next)
			pipe(ctx.pipefd);
		pid = fork();
		if (pid == 0)
			pipe_child(cmd, &ctx);
		last_pid = pid;
		update_fds(&ctx, cmd);
		cmd = cmd->next;
	}
	wait_all(last_pid);
}
