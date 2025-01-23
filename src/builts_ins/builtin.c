/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:17:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/23 14:29:07 by anggalle         ###   ########.fr       */
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
	else if (ft_strncmp(data->ast->value, "cd", ft_strlen(data->ast->value)) == 0)
	{
		ret = cd_builtin(data);
	}
	else if (ft_strncmp(data->ast->value, "export", ft_strlen(data->ast->value)) == 0)
	{
		ret = export_builtin(data);
	}
	return (ret);
}
