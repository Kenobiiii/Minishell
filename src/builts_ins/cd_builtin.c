/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:32:49 by anggalle          #+#    #+#             */
/*   Updated: 2025/06/10 15:24:33 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	cd_handle_no_args(t_data *data)
{
	const char	*home_dir;

	if (!data)
		return (1);
	home_dir = getenv("HOME");
	if (home_dir == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		data->wstatus = 1;
		return (1);
	}
	if (chdir(home_dir) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(home_dir);
		data->wstatus = 1;
		return (1);
	}
	data->wstatus = 0;
	return (1);
}

static int	cd_handle_too_many_args(t_data *data)
{
	ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
	data->wstatus = 1;
	return (1);
}

static int	cd_handle_one_arg(t_data *data, char *path)
{
	if (!data || !path)
		return (1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(path);
		data->wstatus = 1;
		return (1);
	}
	data->wstatus = 0;
	return (1);
}

int	cd_builtin(t_data *data)
{
	if (!data || !data->ast || !data->ast->args)
		return (1);
	if (!data->ast->args[1])
		return (cd_handle_no_args(data));
	else if (data->ast->args[2])
		return (cd_handle_too_many_args(data));
	else
		return (cd_handle_one_arg(data, data->ast->args[1]));
}
