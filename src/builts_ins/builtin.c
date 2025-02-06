/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:17:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/02/06 13:23:44 by paromero         ###   ########.fr       */
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
	else if (ft_strncmp(data->ast->value, "$?", ft_strlen(data->ast->value)) == 0)
	{
		ret = last_cmd_status(data);
	}
	else if (ft_strncmp(data->ast->value, "cd", ft_strlen(data->ast->value)) == 0)
	{
		ret = cd_builtin(data);
	}
	else if (ft_strncmp(data->ast->value, "export", ft_strlen(data->ast->value)) == 0)
	{
		ret = export_builtin(data);
	}
	else if (ft_strncmp(data->ast->value, "unset", ft_strlen(data->ast->value)) == 0)
	{
		ret = unset_builtin(data);
	}
	else if (ft_strncmp(data->ast->value, "env", ft_strlen(data->ast->value)) == 0)
	{
		ret = env_builtin(data);
	}
	else if (ft_strncmp(data->ast->value, "exit", ft_strlen(data->ast->value)) == 0)
	{
		ret = exit_builtin(data);
	}
	return (ret);
}
