#include "minishell.h"

static void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

static char	*try_paths(char *cmd, char **paths)
{
	char	*tmp;
	char	*full;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*result;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = ft_split(get_env("PATH", env), ':');
	if (!paths)
		return (NULL);
	result = try_paths(cmd, paths);
	free_array(paths);
	return (result);
}

void	exec_cmd(t_cmd *cmd, t_env *env)
{
	char	**envp;
	char	*path;
	pid_t	pid;
	int		status;

	signals_exec();
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (setup_redirections(cmd) == -1)
			exit(1);
		path = find_path(cmd->args[0], env);
		if (!path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putendl_fd(cmd->args[0], 2);
			exit(127);
		}
		envp = env_to_array(env);
		execve(path, cmd->args, envp);
		exit(1);
	}
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
}  // function needs to be refactored: 28 lines 

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
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (cmd->next)
		exec_pipeline(cmd, env);
	else if (is_builtin(cmd->args[0]))
		exec_builtin_with_redirs(cmd, env);
	else
		exec_cmd(cmd, *env);
}