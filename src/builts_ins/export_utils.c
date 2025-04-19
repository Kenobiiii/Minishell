/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:23:14 by paromero          #+#    #+#             */
/*   Updated: 2025/04/09 18:24:02 by paromero         ###   ########.fr       */
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

void	print_export_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		printf("declare -x %s\n", matrix[i]);
		i ++;
	}
}

void	print_env_sorted(char **env_matrix)
{
	sort_matrix(env_matrix);
	print_export_matrix(env_matrix);
}
