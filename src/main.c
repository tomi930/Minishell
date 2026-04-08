#include "minishell.h"

int g_exit_status = 0;

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();       // tell readline we're on a new line
    rl_replace_line("", 0); // clear the current input
    rl_redisplay();         // redisplay the prompt
}

int main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env_head;

	env_head = copy_env(envp);
	signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
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
    return (0);
}