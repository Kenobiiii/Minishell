/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:14:24 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/26 14:33:05 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_builtin(t_data *data)
{
	int	i;
	int	exit_code;

	i = 0;
	if (data->ast->args[1])
	{
		while (data->ast->args[1][i])
		{
			if (!ft_isdigit(data->ast->args[1][i]) && (data->ast->args[1][i] != '-')
				&& (data->ast->args[1][i] != '+'))
				exit(2);
			i++;
		}
		if (data->ast->args[2])
		{
			free_innerwhile(data);
			exit (1);
		}
		exit_code = ft_atoi(data->ast->args[1]);
		free_innerwhile(data);
		exit (exit_code);
	}
	else
		data->exit = 1;
	return (1);
}
