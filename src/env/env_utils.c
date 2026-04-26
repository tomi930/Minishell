#include "minishell.h"

void	remove_env(char *key, t_env **head)
{
	t_env	*prev;
	t_env	*current;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static char	*env_entry(t_env *node)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(node->key, "=");
	if (node->value == NULL)
	{
		free(tmp);
		return (NULL);
	}
	result = ft_strjoin(tmp, node->value);
	free(tmp);
	return (result);
}

static int	count_env(t_env *head)
{
	int	count;

	count = 0;
	while (head)
	{
		if (head->value)
			count++;
		head = head->next;
	}
	return (count);
}

char	**env_to_array(t_env *head)
{
	char	**array;
	int		i;

	array = malloc(sizeof(char *) * (count_env(head) + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (head)
	{
		if (head->value)
			array[i++] = env_entry(head);
		head = head->next;
	}
	array[i] = NULL;
	return (array);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
