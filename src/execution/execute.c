#include "minishell.h"

static void	child_exit(t_cmd *cmd, t_env *env, int code)
{
	rl_clear_history();
	if (env)
		free_env(env);
	free_cmd(cmd);
	exit(code);
}

static void	child_exec(t_cmd *cmd, t_env *env)
{
	char	**envp;
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (setup_redirections(cmd) == -1)
		child_exit(cmd, env, 1);
	path = find_path(cmd->args[0], env);
	if (!path)
	{
		errmsg("minishell", cmd->args[0], "command not found");
		child_exit(cmd, env, 127);
	}
	envp = env_to_array(env);
	free_env(env);
	execve(path, cmd->args, envp);
	errno = execve_errno(path);
	errmsg("minishell", cmd->args[0], strerror(errno));
	free(path);
	free_envp(envp);
	if (errno == ENOENT)
		child_exit(cmd, NULL, 127);
	child_exit(cmd, NULL, 126);
}

void	exec_cmd(t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	signals_exec();
	pid = fork();
	if (pid == 0)
		child_exec(cmd, env);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
	signals_interactive();
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}

static void	exec_builtin_with_redirs(t_cmd *cmd, t_env **env)
{
	int	saved_in;
	int	saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (setup_redirections(cmd) == -1)
	{
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		return ;
	}
	g_exit_status = exec_builtin(cmd, env);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

void	execute(t_cmd *cmd, t_env **env)
{
	if (!cmd)
		return ;
	if (!cmd->args || !cmd->args[0])
	{
		if (cmd->redirs)
			apply_redirs_only(cmd);
		return ;
	}
	if (cmd->next)
		exec_pipeline(cmd, env);
	else if (is_builtin(cmd->args[0]))
		exec_builtin_with_redirs(cmd, env);
	else
		exec_cmd(cmd, *env);
}
