#include "minishell.h"

static int	is_valid_identifier(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	set_export_var(char *arg, t_env **env)
{
	char	*eq;
	char	*key;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		set_env(key, eq + 1, env);
		free(key);
	}
	else
		set_env(arg, NULL, env);
}

int	builtin_export(t_cmd *cmd, t_env **env)
{
	int	i;
	int	ret;

	if (!cmd->args[1])
		return (print_export(*env), 0);
	i = 1;
	ret = 0;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			errmsg("export", cmd->args[i], "not a valid identifier");
			ret = 1;
		}
		else
			set_export_var(cmd->args[i], env);
		i++;
	}
	return (ret);
}
