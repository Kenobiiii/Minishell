/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:32:49 by anggalle          #+#    #+#             */
/*   Updated: 2025/06/10 20:50:20 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_cd_error(t_data *data, char *dir, char *old_pwd,
		char *to_free)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	perror(dir);
	data->wstatus = 1;
	free(old_pwd);
	if (to_free)
		free(to_free);
	return (1);
}

static int	cd_handle_no_args(t_data *data)
{
	char	*home_dir;
	char	*old_pwd;

	if (!data)
		return (1);
	home_dir = find_variable_in_env(data, "HOME=");
	if (home_dir == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		data->wstatus = 1;
		return (1);
	}
	old_pwd = get_current_pwd(data);
	if (chdir(home_dir) == -1)
		return (handle_cd_error(data, home_dir, old_pwd, home_dir));
	update_pwd_variables(data, old_pwd);
	free(home_dir);
	free(old_pwd);
	data->wstatus = 0;
	return (1);
}

static int	cd_handle_one_arg(t_data *data, char *path)
{
	char	*old_pwd;

	if (!data || !path)
		return (1);
	old_pwd = get_current_pwd(data);
	if (chdir(path) == -1)
		return (handle_cd_error(data, path, old_pwd, NULL));
	update_pwd_variables(data, old_pwd);
	free(old_pwd);
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
