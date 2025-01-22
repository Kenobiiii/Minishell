/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:08:21 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/22 14:30:19 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin(t_data *data)
{
	char	*path;

	if (data->ast->args[1])
	{
		printf("Syntax error\n");
		return (1);
	}
	path = find_variable_in_env(data, "PWD=");
	ft_putstr_fd(path, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (1);
}
