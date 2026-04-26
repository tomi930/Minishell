#include "minishell.h"

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
