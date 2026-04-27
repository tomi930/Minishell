#include "minishell.h"

static int	cd_error(char *path)
{
	errmsg("minishell: cd", path, strerror(errno));
	return (1);
}

static void	update_pwd(t_env **env)
{
	char	buf[4096];

	set_env("OLDPWD", get_env("PWD", *env), env);
	if (getcwd(buf, sizeof(buf)))
		set_env("PWD", buf, env);
}

int	builtin_cd(t_cmd *cmd, t_env **env)
{
	char	*path;

	if (!cmd->args[1])
		path = get_env("HOME", *env);
	else
		path = cmd->args[1];
	if (!path)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (1);
	}
	if (chdir(path) != 0)
		return (cd_error(path));
	update_pwd(env);
	return (0);
}
