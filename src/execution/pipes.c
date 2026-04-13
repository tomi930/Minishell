#include "minishell.h"

static void	pipe_child(t_cmd *cmd, t_env **env, int prev_fd, int *pipefd)
{
	char	*path;
	char	**envp;

	if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (cmd->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (prev_fd != -1)
		close(prev_fd);
	setup_redirections(cmd);
	if (is_builtin(cmd->args[0]))
		exit(exec_builtin(cmd, env));
	path = find_path(cmd->args[0], *env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		exit(127);
	}
	envp = env_to_array(*env);
	execve(path, cmd->args, envp);
	exit(1);
}


void	exec_pipeline(t_cmd *cmd, t_env **env)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	last_pid;
	pid_t	pid;
	int		status;

	prev_fd = -1;
	last_pid = -1;
	while (cmd)
	{
		if (cmd->next)
			pipe(pipefd);
		pid = fork();
		if (pid == 0)
			pipe_child(cmd, env, prev_fd, pipefd);
		last_pid = pid;
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		cmd = cmd->next;
	}
	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (pid == last_pid)
			g_exit_status = WEXITSTATUS(status);
	}
}