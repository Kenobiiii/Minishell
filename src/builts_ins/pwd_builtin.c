/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:08:21 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/29 16:51:16 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd_builtin(t_data *data)
{
	char	*path;

	path = find_variable_in_env(data, "PWD=");
	if (path == NULL)
		ft_putstr_fd("minishell: no path for PWD found\n", STDERR_FILENO);
	else
	{
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	free(path);
	data->wstatus = 0;
	return (1);
}
