/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:14:24 by anggalle          #+#    #+#             */
/*   Updated: 2025/04/09 18:32:45 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_builtin(t_data *data, t_ast *ast)
{
	int	i;
	int	exit_code;

	i = 0;
	if (data->ast->args[1])
	{
		while (data->ast->args[1][i])
		{
			if (!ft_isdigit(ast->args[1][i]) && (ast->args[1][i] != '-')
				&& (ast->args[1][i] != '+'))
				exit(2);
			i++;
		}
		if (ast->args[2])
		{
			free_while(data);
			exit (1);
		}
		exit_code = ft_atoi(ast->args[1]);
		free_while(data);
		exit (exit_code);
	}
	else
		data->exit = 1;
	return (1);
}
