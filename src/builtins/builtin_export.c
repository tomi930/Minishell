#include "minishell.h"

static int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static t_env	**env_to_sorted_array(t_env *env, int count)
{
	t_env	**arr;
	t_env	*tmp;
	int		i;
	int		j;

	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < count && env)
	{
		arr[i++] = env;
		env = env->next;
	}
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	return (arr);
}

static void	print_env_entry(t_env *e)
{
	ft_putstr_fd("export ", 1);
	ft_putstr_fd(e->key, 1);
	if (e->value)
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(e->value, 1);
		ft_putstr_fd("\"", 1);
	}
	ft_putchar_fd('\n', 1);
}

static void	print_export(t_env *env)
{
	t_env	**arr;
	int		count;
	int		i;

	count = env_count(env);
	arr = env_to_sorted_array(env, count);
	if (!arr)
		return ;
	i = 0;
	while (i < count)
		print_env_entry(arr[i++]);
	free(arr);
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
	int		ret;

	if (!cmd->args[1])
		return (print_export(*env), 0);
	i = 1;
	ret = 0;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			ft_putstr_fd("export: not a valid identifier: ", 2);
			ft_putendl_fd(cmd->args[i], 2);
			ret = 1;
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
	return (ret);
}

//needs refactoring