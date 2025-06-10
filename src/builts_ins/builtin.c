/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:17:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/06/10 15:24:38 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	execute_basic_builtins(t_data *data, char *line, char *wstatus)
{
	if (ft_strncmp(line, "echo", 5) == 0)
		return (echo_builtin(data));
	else if (ft_strncmp(line, "pwd", 4) == 0)
		return (pwd_builtin(data));
	else if (ft_strncmp(line, wstatus, ft_strlen(line)) == 0)
		return (last_cmd_status(data));
	else if (ft_strncmp(line, "cd", 3) == 0)
		return (cd_builtin(data));
	return (-1);
}

static int	execute_advanced_builtins(t_data *data, char *line)
{
	if (ft_strncmp(line, "export", 7) == 0)
		return (export_builtin(data));
	else if (ft_strncmp(line, "unset", 6) == 0)
		return (unset_builtin(data));
	else if (ft_strncmp(line, "env", 4) == 0)
		return (env_builtin(data));
	else if (ft_strncmp(line, "exit", 5) == 0)
		return (exit_builtin(data, data->ast));
	else if (ft_strncmp(line, "$EMPTY", 6) == 0)
		return (printf("\n"));
	return (0);
}

static int	execute_builtin_commands(t_data *data, char *line, char *wstatus)
{
	int	ret;

	ret = execute_basic_builtins(data, line, wstatus);
	if (ret != -1)
		return (ret);
	return (execute_advanced_builtins(data, line));
}

int	is_builtins(t_data *data, char *line)
{
	int		ret;
	char	*wstatus;

	if (!data || !line)
		return (0);
	wstatus = ft_itoa(data->wstatus);
	if (!wstatus)
		return (0);
	ret = execute_builtin_commands(data, line, wstatus);
	free (wstatus);
	return (ret);
}
