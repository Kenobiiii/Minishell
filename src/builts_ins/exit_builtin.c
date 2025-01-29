/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:14:24 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/29 18:19:43 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_builtin(t_data *data)
{
	if (data->ast->args[1])
	{
		//Manejar error
		printf("Error en la introducción de comandos\n");
	}
	data->exit = 1;
	return (1);
}