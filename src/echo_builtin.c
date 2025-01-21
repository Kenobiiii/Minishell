/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:09:01 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/21 20:04:22 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_builtin(t_data *data)
{
	int i;
	int	print_new_line;

	print_new_line = 0;
	i = 0;
	while (data->ast->args[i])
	{
		if (ft_strncmp(data->ast->args[i], "-n", 2) == 0)
		{
			if (ft_strncmp(data->ast->args[i], "-n", ft_strlen(data->ast->args[i])) == 0)
				print_new_line = 1;
			else
			{
				ft_putstr_fd(data->ast->args[i], STDOUT_FILENO);
				print_new_line = 0;
			}
		}
		else
		{
			ft_putstr_fd(data->ast->args[i], STDOUT_FILENO);
			if (print_new_line)
				ft_putchar_fd("\n", STDOUT_FILENO);
		}
		i ++;
	}
}
