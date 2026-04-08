#include "minishell.h"

int g_exit_status = 0;


int main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env_head;

	env_head = copy_env(envp);
	signals_interactive();
    while (1)
    {
        line = readline("minishell> ");
        if (!line)
		{
			write(1,"exit\n", 5);
            break;
		}
        if (*line)
            add_history(line);
        free(line);
    }
	free_env(env_head);
    return (g_exit_status);
}