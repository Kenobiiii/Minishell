/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:14:24 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/21 12:56:07 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_builtin(t_data *data)
{
	if (data->ast->args[1])
	{
		//TODO Manejar error
		printf("Error en la introducción de comandos\n");
	}
	else
		data->exit = 1;
	return (1);
}
