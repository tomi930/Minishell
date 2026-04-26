#include "minishell.h"

char	*get_env_key(char *environ)
{
	char	*eq;

	eq = ft_strchr(environ, '=');
	return (ft_substr(environ, 0, eq - environ));
}

char	*get_env_value(char *environ)
{
	char	*eq;

	eq = ft_strchr(environ, '=');
	return (ft_strdup(eq + 1));
}

t_env	*copy_env(char **environ)
{
	t_env	*head;
	t_env	*current;
	int		i;

	if (!environ || !environ[0])
		return (NULL);
	i = 0;
	head = malloc(sizeof(t_env));
	current = head;
	while (environ[i])
	{
		current->key = get_env_key(environ[i]);
		current->value = get_env_value(environ[i]);
		current->next = NULL;
		if (environ[i + 1])
		{
			current->next = malloc(sizeof(t_env));
			current = current->next;
		}
		i++;
	}
	return (head);
}

char	*get_env(char *key_string, t_env *head)
{
	while (head)
	{
		if (ft_strcmp(key_string, head->key) == 0)
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

void	free_env(t_env *head)
{
	t_env	*holder;

	while (head)
	{
		holder = head;
		free(head->key);
		free(head->value);
		head = head->next;
		free(holder);
	}
}
