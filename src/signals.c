#include "minishell.h"

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();       // tell readline we're on a new line
    rl_replace_line("", 0); // clear the current input
    rl_redisplay();         // redisplay the prompt
}

void	signals_interactive(void)   // at the prompt
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
}

void	signals_exec(void)          // while a child is running
{
    signal(SIGINT, SIG_IGN);   // let child handle it naturally
    signal(SIGQUIT, SIG_IGN);  // Ctrl+\ works normally too
}
