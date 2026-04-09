#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

extern int g_exit_status;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_redir_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type    type;
    char            *target;
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
	char			**args;
	char			*infile; //probably will delete later bc of redirs
	char			*outfile; //probably will delete later bc of redirs
	int				append; //probably will delete later bc of redirs
	int				is_heredoc; //probably will delete later bc of redirs
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

/* env_init.c */
char	*get_env_key(char *environ);
char	*get_env_value(char *environ);
t_env	*copy_env(char **environ);
char	*get_env(char *key_string, t_env *head);
void	free_env(t_env *head);

/* env_utils.c */
void	set_env(char *key, char *value, t_env **head);
void	remove_env(char *key, t_env **head);
char	**env_to_array(t_env *head);

/* signals.c */
void	handle_sigint(int sig);
void	signals_interactive(void);
void	signals_exec(void);

/* execute.c */
char	*find_path(char *cmd, t_env *env);
void	exec_cmd(t_cmd *cmd, t_env *env);
void	execute(t_cmd *cmd, t_env **env);

/* pipes.c */
void	exec_pipeline(t_cmd *cmd, t_env **env);

/* redirection.c */
int		setup_redirections(t_cmd *cmd);

/* builtins.c */
int		is_builtin(char *cmd);
int		exec_builtin(t_cmd *cmd, t_env **env);

/* builtin_echo.c */
int		builtin_echo(t_cmd *cmd);

/* builtin_cd.c */
int		builtin_cd(t_cmd *cmd, t_env **env);

/* builtin_env.c */
int		builtin_env(t_env *env);

/* builtin_export.c */
int		builtin_export(t_cmd *cmd, t_env **env);
int		builtin_unset(t_cmd *cmd, t_env **env);
int		builtin_pwd(void);
int		builtin_exit(t_cmd *cmd);

/* parsing */
t_cmd	*parse(char *line, t_env *env);
void	free_cmd(t_cmd *cmd);
int		has_unclosed_quotes(const char *line);
char	**tokenize(char *line);
void	free_tokens(char **tokens);
char	*expand_token(const char *token, t_env *env);
char	*strip_quotes(const char *token);

#endif