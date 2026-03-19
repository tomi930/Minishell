#include "minishell.h"

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	builtin_exit(t_cmd *cmd)
{
	int	exit_code;

	if (cmd->args[1] && cmd->args[2])
	{
		ft_printf("exit: too many arguments\n");
		return (1);
	}
	if (!cmd->args[1])
		exit(0);
	exit_code = ft_atoi(cmd->args[1]);
	if (exit_code < 0 || exit_code > 255)
	{
		ft_printf("exit: numeric argument required\n");
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
	int	i;

	if (!cmd->args[1])
		return (print_export(*env));
	i = 1;
	while (cmd->args[i])
	{
		if (is_valid_identifier(cmd->args[i]))
			add_env(cmd->args[i], env);
		else
			ft_printf("export: `%s': not a valid identifier\n", cmd->args[i]);
		i++;
	}
	return (0);
}