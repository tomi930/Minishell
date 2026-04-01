#include "minishell.h"

static void	print_export(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->key, 1);
		if (env->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		env = env->next;
	}
}

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

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}

int	builtin_exit(t_cmd *cmd)
{
	int	exit_code;

	if (cmd->args[1] && cmd->args[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	if (!cmd->args[1])
		exit(0);
	exit_code = ft_atoi(cmd->args[1]);
	if (exit_code < 0 || exit_code > 255)
	{
		ft_putendl_fd("exit: numeric argument required", 2);
		exit(255);
	}
	exit(exit_code);
	return (0);
}

int	builtin_unset(t_cmd *cmd, t_env **env)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		remove_env(cmd->args[i], env);
		i++;
	}
	return (0);
}

int	builtin_export(t_cmd *cmd, t_env **env)
{
	char	*eq;
	char	*key;
	int		i;

	if (!cmd->args[1])
		return (print_export(*env), 0);
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			ft_putstr_fd("export: not a valid identifier: ", 2);
			ft_putendl_fd(cmd->args[i], 2);
		}
		else
		{
			eq = ft_strchr(cmd->args[i], '=');
			if (eq)
			{
				key = ft_substr(cmd->args[i], 0, eq - cmd->args[i]);
				set_env(key, eq + 1, env);
				free(key);
			}
			else
				set_env(cmd->args[i], NULL, env);
		}
		i++;
	}
	return (0);
}