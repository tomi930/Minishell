void	set_env(char *key, char *value, t_env **head)
{
	t_env	*current;
	t_env	*new_node;

	current = *head;
	while (current)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (current)
		current->next = new_node;
	else
		*head = new_node;
}

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
		current = current->next;
	}
}

static char	*env_entry(t_env *node)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(node->key, "=");
	result = ft_strjoin(tmp, node->value);
	free(tmp);
	return (result);
}

char	**env_to_array(t_env *head)
{
	char	**array;
	t_env	*current;
	int		count;
	int		i;

	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (head)
	{
		array[i++] = env_entry(head);
		head = head->next;
	}
	array[i] = NULL;
	return (array);
}
