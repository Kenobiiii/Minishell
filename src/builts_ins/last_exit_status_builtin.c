/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_exit_status_builtin.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 13:23:25 by paromero          #+#    #+#             */
/*   Updated: 2025/05/14 19:06:55 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	last_cmd_status(t_data *data)
{
	char	*status_str;

	status_str = ft_itoa(data->wstatus);
	ft_putstr_fd(status_str, 2);
	ft_putstr_fd(": command not found\n", 2);
	free(status_str);
	data->wstatus = 127;
	return (1);
}
