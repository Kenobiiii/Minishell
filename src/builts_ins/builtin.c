/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:17:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/04/09 18:32:55 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtins(t_data *data, char *line)
{
	int		ret;
	char	*wstatus;

	wstatus = ft_itoa(data->wstatus);
	ret = 0;
	if (ft_strncmp(line, "echo", 5) == 0)
		ret = echo_builtin(data);
	else if (ft_strncmp(line, "pwd", 4) == 0)
		ret = pwd_builtin(data);
	else if (ft_strncmp(line, wstatus, ft_strlen(line)) == 0)
		ret = last_cmd_status(data);
	else if (ft_strncmp(line, "cd", 3) == 0)
		ret = cd_builtin(data);
	else if (ft_strncmp(line, "export", 7) == 0)
		ret = export_builtin(data);
	else if (ft_strncmp(line, "unset", 6) == 0)
		ret = unset_builtin(data);
	else if (ft_strncmp(line, "env", 4) == 0)
		ret = env_builtin(data);
	else if (ft_strncmp(line, "exit", 5) == 0)
		ret = exit_builtin(data, data->ast);
	else if (ft_strncmp(line, "$EMPTY", 6) == 0)
		ret = printf("\n");
	free (wstatus);
	return (ret);
}
