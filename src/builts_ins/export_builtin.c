/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:34:49 by anggalle          #+#    #+#             */
/*   Updated: 2025/04/09 18:23:35 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(char *arg)
{
	int	i;

	i = 1;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_env	*exist_identifier(t_env **env, char *args)
{
	t_env	*current;
	int		equal_pos;

	equal_pos = 0;
	while (args[equal_pos])
	{
		if (args[equal_pos] == '=')
			break ;
		equal_pos ++;
	}
	equal_pos = 0;
	while (args[equal_pos])
	{
		if (args[equal_pos] == '=')
			break ;
		equal_pos ++;
	}
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->value, args, equal_pos) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	add_or_update_env(t_env	**env, char *args)
{
	t_env	*env_node;
	t_env	*new_node;
	t_env	*last;

	env_node = exist_identifier(env, args);
	if ((env_node))
	{
		free(env_node->value);
		env_node->value = ft_strdup(args);
	}
	else
	{
		new_node = create_env_node(ft_strdup(args));
		if (!*env)
		{
			*env = new_node;
		}
		else
		{
			last = *env;
			while (last->next)
				last = last->next;
			last->next = new_node;
		}
	}
}

int	export_builtin(t_data *data)
{
	char	**env_matrix;
	int		i;

	i = 1;
	env_matrix = list_to_array(data->env);
	if (!data->ast->args[1])
	{
		print_env_sorted(env_matrix);
		return (1);
	}
	while (data->ast->args[i])
	{
		if (!is_valid_identifier(data->ast->args[i]))
		{
			data->wstatus = 1;
			return (1);
		}
		else
			add_or_update_env(&data->env, data->ast->args[i]);
		i ++;
	}
	return (1);
}
