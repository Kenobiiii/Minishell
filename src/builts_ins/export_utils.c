/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:23:14 by paromero          #+#    #+#             */
/*   Updated: 2025/05/31 11:31:52 by paromero         ###   ########.fr       */
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

void	print_env_sorted(char **env_matrix)
{
	int	i;

	i = 0;
	sort_matrix(env_matrix);
	while (env_matrix[i])
	{
		printf("declare -x %s\n", env_matrix[i]);
		i ++;
	}
}

int	is_valid_identifier(char *arg)
{
	int	i;

	if (!arg || !arg[0])
		return (0);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
