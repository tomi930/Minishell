#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <sys/stat.h>
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

typedef struct s_pipe_ctx
{
	t_cmd	*head;
	t_env	**env;
	int		prev_fd;
	int		pipefd[2];
}	t_pipe_ctx;

typedef struct s_expand
{
	char	*out;
	int		len;
	int		i;
	char	quote;
}	t_expand;

/* error.c */
void	errmsg(char *prefix, char *name, char *suffix);
int		execve_errno(char *path);

/* env_init.c */
char	*get_env_key(char *environ);
char	*get_env_value(char *environ);
t_env	*copy_env(char **environ);
char	*get_env(char *key_string, t_env *head);
void	free_env(t_env *head);

/* env_set.c */
void	set_env(char *key, char *value, t_env **head);

/* env_utils.c */
void	remove_env(char *key, t_env **head);
char	**env_to_array(t_env *head);
void	free_envp(char **envp);

/* signals.c */
void	handle_sigint(int sig);
void	signals_interactive(void);
void	signals_exec(void);

/* find_path.c */
char	*find_path(char *cmd, t_env *env);

/* execute.c */
void	exec_cmd(t_cmd *cmd, t_env *env);
void	execute(t_cmd *cmd, t_env **env);

/* pipes.c */
void	exec_pipeline(t_cmd *cmd, t_env **env);

/* pipes_utils.c */
void	pipe_exit(t_pipe_ctx *ctx, int code);
void	pipe_setup_fds(t_cmd *cmd, t_pipe_ctx *ctx);

/* redirection.c */
int		setup_redirections(t_cmd *cmd);
void	apply_redirs_only(t_cmd *cmd);

/* builtins.c */
int		is_builtin(char *cmd);
int		exec_builtin(t_cmd *cmd, t_env **env);

/* builtin_echo.c */
int		builtin_echo(t_cmd *cmd);

/* builtin_cd.c */
int		builtin_cd(t_cmd *cmd, t_env **env);

/* builtin_env.c */
int		builtin_env(t_env *env);

/* export_print.c */
void	print_export(t_env *env);

/* builtin_export.c */
int		builtin_export(t_cmd *cmd, t_env **env);

/* builtin_unset.c */
int		builtin_unset(t_cmd *cmd, t_env **env);

/* builtin_pwd.c */
int		builtin_pwd(void);

/* builtin_exit.c */
int		builtin_exit(t_cmd *cmd, t_env **env);

/* parsing */
t_cmd	*parse(char *line, t_env *env);
void	free_cmd(t_cmd *cmd);
int		has_unclosed_quotes(const char *line);
char	**tokenize(char *line);
void	free_tokens(char **tokens);
char	*expand_token(const char *token, t_env *env);
char	*strip_quotes(const char *token);
int		token_is_space(char c);
int		token_is_op_char(char c);
int		add_token(char ***tokens, int *count, int *cap, char *token);
char	*read_operator(char *line, int *i);
char	*read_word(char *line, int *i);
int		parse_is_op(const char *t);
int		parse_is_redir(const char *t);
t_cmd	*parse_new_cmd(void);
int		parse_append_arg(t_cmd *cmd, char *arg);
int		set_redirection(t_cmd *cmd, char *op, char *target, t_env *env);
int		is_name_start(char c);
int		is_name_char(char c);
int		append_char(t_expand *exp, char c);
int		append_str(t_expand *exp, const char *s);
int		append_exp_env(t_expand *exp, const char *token, t_env *env);
int		token_has_quotes(const char *token);
char	*read_heredoc(char *delimiter, int expand, t_env *env);

#endif
