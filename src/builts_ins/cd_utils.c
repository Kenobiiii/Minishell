/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 20:45:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 20:50:20 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_current_pwd(t_data *data)
{
	char	*current_pwd;
	char	cwd[PATH_MAX];

	current_pwd = find_variable_in_env(data, "PWD=");
	if (!current_pwd)
	{
		if (getcwd(cwd, sizeof(cwd)))
			current_pwd = ft_strdup(cwd);
		else
			current_pwd = ft_strdup("");
	}
	return (current_pwd);
}

void	update_pwd_variables(t_data *data, char *old_pwd)
{
	char	*current_pwd;
	char	*pwd_var;
	char	*oldpwd_var;
	char	cwd[PATH_MAX];

	current_pwd = getcwd(cwd, sizeof(cwd));
	if (!current_pwd)
		return ;
	pwd_var = ft_strcat("PWD=", current_pwd);
	if (pwd_var)
	{
		add_or_update_env(&data->env, pwd_var);
		free(pwd_var);
	}
	if (old_pwd)
	{
		oldpwd_var = ft_strcat("OLDPWD=", old_pwd);
		if (oldpwd_var)
		{
			add_or_update_env(&data->env, oldpwd_var);
			free(oldpwd_var);
		}
	}
}

int	cd_handle_too_many_args(t_data *data)
{
	ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
	data->wstatus = 1;
	return (1);
}
