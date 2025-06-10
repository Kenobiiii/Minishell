/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:34:49 by anggalle          #+#    #+#             */
/*   Updated: 2025/06/10 20:31:29 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*exist_identifier(t_env **env, char *args)
{
	t_env	*current;
	int		equal_pos;

	if (!env || !args)
		return (NULL);
	equal_pos = 0;
	while (args[equal_pos] && args[equal_pos] != '=')
		equal_pos++;
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->value, args, equal_pos) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static void	handle_append_assignment(t_env **env, char *args)
{
	char	*var_name;
	char	*append_value;
	char	*search_key;
	t_env	*env_node;

	var_name = get_var_name_from_append(args);
	append_value = get_append_value(args);
	if (!var_name || !append_value)
	{
		free(var_name);
		free(append_value);
		return ;
	}
	search_key = ft_strjoin(var_name, "=");
	if (!search_key)
	{
		free(append_value);
		return ;
	}
	env_node = exist_identifier(env, search_key);
	if (env_node)
		handle_existing_env_append(env_node, search_key, append_value);
	else
		handle_new_env_append(env, search_key, append_value);
	free(append_value);
}

void	add_or_update_env(t_env	**env, char *args)
{
	t_env	*env_node;

	if (is_append_assignment(args))
	{
		handle_append_assignment(env, args);
		return ;
	}
	env_node = exist_identifier(env, args);
	if ((env_node))
	{
		free(env_node->value);
		env_node->value = ft_strdup(args);
	}
	else
	{
		add_new_env_node(env, args);
	}
}

static void	process_export_arguments(t_data *data, int *has_error)
{
	int	i;

	i = 1;
	while (data->ast->args[i])
	{
		if (!is_valid_identifier(data->ast->args[i]))
		{
			ft_putstr_fd("-minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(data->ast->args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			*has_error = 1;
		}
		else
			add_or_update_env(&data->env, data->ast->args[i]);
		i++;
	}
}

int	export_builtin(t_data *data)
{
	char	**env_matrix;
	int		has_error;

	has_error = 0;
	if (!data->ast->args[1])
	{
		env_matrix = list_to_array(data->env);
		print_env_sorted(env_matrix);
		free_array((const char **)env_matrix);
		data->wstatus = 0;
		return (1);
	}
	process_export_arguments(data, &has_error);
	if (has_error)
		data->wstatus = 1;
	else
		data->wstatus = 0;
	return (1);
}
