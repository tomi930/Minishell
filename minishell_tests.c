#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();       // tell readline we're on a new line
    rl_replace_line("", 0); // clear the current input
    rl_redisplay();         // redisplay the prompt
}

// In main, before your loop:

int main(void)
{
    char *line;

    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
    while (1)
    {
        line = readline("minishell> ");
        if (!line)
            break;
        if (*line)
            add_history(line);
        printf("You typed: %s\n", line);
        free(line);
    }
    return (0);
}
