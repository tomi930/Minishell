#include "minishell.h"

int	g_exit_status = 0;

static void	process_line(char *line, t_env **env_head)
{
	t_cmd	*cmd;

	add_history(line);
	cmd = parse(line, *env_head);
	if (cmd)
	{
		execute(cmd, env_head);
		free_cmd(cmd);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env_head;

	(void)argc;
	(void)argv;
	env_head = copy_env(envp);
	signals_interactive();
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*line)
			process_line(line, &env_head);
		free(line);
	}
	rl_clear_history();
	free_env(env_head);
	return (g_exit_status);
}
