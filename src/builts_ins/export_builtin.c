/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:34:49 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/23 14:25:07 by anggalle         ###   ########.fr       */
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

int	export_builtin(t_data *data)
{
	char	**env_matrix;

	env_matrix = list_to_array(data->env);
	if (!data->ast->args[1])
	{
		print_env_sorted(env_matrix);
	}
	return (1);
}