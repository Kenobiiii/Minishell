/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 20:30:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 20:31:29 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_append_assignment(char *args)
{
	int	i;

	i = 0;
	while (args[i] && args[i] != '=')
	{
		if (args[i] == '+' && args[i + 1] == '=')
			return (1);
		i++;
	}
	return (0);
}

char	*get_var_name_from_append(char *args)
{
	int		i;
	char	*var_name;

	i = 0;
	while (args[i] && args[i] != '+')
		i++;
	var_name = ft_substr(args, 0, i);
	return (var_name);
}

char	*get_append_value(char *args)
{
	int	i;

	i = 0;
	while (args[i] && !(args[i] == '+' && args[i + 1] == '='))
		i++;
	if (args[i] == '+' && args[i + 1] == '=')
		return (ft_strdup(args + i + 2));
	return (NULL);
}

char	*handle_existing_var_append(t_env *env_node, char *append_value)
{
	char	*current_value;
	char	*temp_current;
	char	*new_value;

	current_value = ft_strchr(env_node->value, '=');
	if (current_value)
		current_value++;
	else
		current_value = "";
	temp_current = ft_strdup(current_value);
	if (!temp_current)
		return (NULL);
	new_value = ft_strjoin(temp_current, append_value);
	return (new_value);
}

void	handle_existing_env_append(t_env *env_node, char *search_key,
			char *append_value)
{
	char	*new_value;

	new_value = handle_existing_var_append(env_node, append_value);
	if (new_value)
	{
		free(env_node->value);
		env_node->value = ft_strjoin(search_key, new_value);
		free(new_value);
	}
	else
		free(search_key);
}
