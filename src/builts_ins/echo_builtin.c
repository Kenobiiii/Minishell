/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:09:01 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/23 11:36:29 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	echo_builtin(t_data *data)
{
	int i;
	int j;
	int	print_new_line;

	print_new_line = 1;
	i = 1;
	if (data->ast->args[1])
	{
		while (data->ast->args[i])
		{
			if (ft_strncmp(data->ast->args[i], "-n", 2) == 0)
			{
				if (ft_strncmp(data->ast->args[i], "-n", ft_strlen(data->ast->args[i])) == 0)
					print_new_line = 0;
				else
				{
					j = 2;
					while (data->ast->args[i][j] == 'n')
						j ++;
					if (data->ast->args[i][j])
					{
						ft_putstr_fd(data->ast->args[i], STDOUT_FILENO);
						if (data->ast->args[i + 1])
							ft_putstr_fd(" ", STDOUT_FILENO);
						print_new_line = 1;
					}else
					{
						print_new_line = 0;
					}
				}
			}
			else
			{
				ft_putstr_fd(data->ast->args[i], STDOUT_FILENO);
				if (data->ast->args[i + 1])
						ft_putstr_fd(" ", STDOUT_FILENO);
			}
			i ++;
		}
		if (print_new_line)
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (1);
}
