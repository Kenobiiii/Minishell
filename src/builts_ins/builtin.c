/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:17:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/02/11 17:59:49 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtins(t_data *data, char *line)
{
	int	ret;

	ret = 0;
	if (ft_strncmp(line, "echo", ft_strlen(line)) == 0)
		ret = echo_builtin(data);
	else if (ft_strncmp(line, "pwd", ft_strlen(line)) == 0)
		ret = pwd_builtin(data);
	else if (ft_strncmp(line, "$?", ft_strlen(line)) == 0)
		ret = last_cmd_status(data);
	else if (ft_strncmp(line, "cd", ft_strlen(line)) == 0)
		ret = cd_builtin(data);
	else if (ft_strncmp(line, "export", ft_strlen(line)) == 0)
		ret = export_builtin(data);
	else if (ft_strncmp(line, "unset", ft_strlen(line)) == 0)
		ret = unset_builtin(data);
	else if (ft_strncmp(line, "env", ft_strlen(line)) == 0)
		ret = env_builtin(data);
	else if (ft_strncmp(line, "exit", ft_strlen(line)) == 0)
		ret = exit_builtin(data);
	return (ret);
}
