#include "minishell.h"

static int	is_numeric(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i = 1;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	exit_numeric_error(char *arg)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	exit(2);
}

int	builtin_exit(t_cmd *cmd)
{
	long	exit_code;

	if (cmd->args[1] && cmd->args[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	if (!cmd->args[1])
		exit(g_exit_status);
	if (!is_numeric(cmd->args[1]))
		exit_numeric_error(cmd->args[1]);
	exit_code = ft_atol(cmd->args[1]);
	exit(((exit_code % 256) + 256) % 256);
	return (0);
}
