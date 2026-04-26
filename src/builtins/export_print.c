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

static void	bubble_sort(t_env **arr, int count)
{
	t_env	*tmp;
	int		i;
	int		j;

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
}

static t_env	**env_to_sorted_array(t_env *env, int count)
{
	t_env	**arr;
	int		i;

	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < count && env)
	{
		arr[i++] = env;
		env = env->next;
	}
	bubble_sort(arr, count);
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

void	print_export(t_env *env)
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
