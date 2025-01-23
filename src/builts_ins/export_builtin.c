/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:34:49 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/23 15:30:34 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_matrix(char **matrix)
{
	int		i;
	int		j;
	int		length;
	char	*swap;

	i = 0;
	j = 0;
	length = 0;
	while (matrix[length] != NULL)
		length++;
	while (i < length - 1)
	{
		j = 0;
		while (j < length - i - 1)
		{
			if (ft_strncmp(matrix[j], matrix[j + 1], ft_strlen(matrix[j])) > 0)
			{
				swap = matrix[j];
				matrix[j] = matrix[j + 1];
				matrix[j + 1] = swap;
			}
			j ++;
		}
		i ++;
	}
}

void	print_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		printf("%s\n", matrix[i]);
		i ++;
	}
}

void	print_env_sorted(char **env_matrix)
{
	sort_matrix(env_matrix);
	print_matrix(env_matrix);
}

int is_valid_identifier(char *arg)
{
	int	i;

	i = 1;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	while (arg[i] && arg[i] != '=') {
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
			break;	
		equal_pos ++;
	}
	equal_pos = 0;
	while (args[equal_pos])
	{
		if (args[equal_pos] == '=')
			break;	
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

	if ((env_node = exist_identifier(env, args)))
	{
		free(env_node->value);
		env_node->value = ft_strdup(args);
	}
	else
	{
		new_node = create_node(ft_strdup(args));
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

	i = 0;
	env_matrix = list_to_array(data->env);
	if (!data->ast->args[1])
	{
		print_env_sorted(env_matrix);
	}
	while (data->ast->args[i])
	{
		if (!is_valid_identifier(data->ast->args[i]))
		{
			printf("export: not a valid identifier: %s\n", data->ast->args[i]);
			return (1);
		}
		if (ft_strchr(data->ast->args[i], '='))
			add_or_update_env(&data->env, data->ast->args[i]);
		i ++;
	}
	return (1);
}
