/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:17:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/23 11:36:29 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtins(t_data *data)
{
	int ret;

	ret = 0;
	if (ft_strncmp(data->ast->value, "echo", ft_strlen(data->ast->value)) == 0)
	{
		ret = echo_builtin(data);
	}
	else if (ft_strncmp(data->ast->value, "pwd", ft_strlen(data->ast->value)) == 0)
	{
		ret = pwd_builtin(data);
	}
	return (ret);
}
