#include "minishell.h"

static void	append_env(char *key, char *value, t_env *current, t_env **head)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (current)
		current->next = new_node;
	else
		*head = new_node;
}

void	set_env(char *key, char *value, t_env **head)
{
	t_env	*current;

	current = *head;
	while (current)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	append_env(key, value, current, head);
}
