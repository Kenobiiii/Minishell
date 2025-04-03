/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:59:22 by anggalle          #+#    #+#             */
/*   Updated: 2025/04/01 11:45:53 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path_in_env(t_data *data) //EN TEORÍA QUITAR
{
	t_env	*aux_env;

	aux_env = data->env;
	while (aux_env->value)
	{
		if (ft_strncmp(aux_env->value, "PATH=", 5) == 0)
		{
			return (aux_env->value);
		}
		aux_env = aux_env->next;
	}
	return (NULL);
}

char	*find_variable_in_env(t_data *data, char *var)
{
	t_env	*aux_env;
	char	*env_var;

	aux_env = data->env;
	while (aux_env->value)
	{
		if (ft_strncmp(aux_env->value, var, ft_strlen(var)) == 0)
		{
			env_var = ft_strdup(aux_env->value + ft_strlen(var));
			return (env_var);
		}
		aux_env = aux_env->next;
	}
	return (NULL);
}

void	free_cmd_path(char	**matrix,	char	*cmd)
{
	free_matrix(matrix);
	free (cmd);
}

char	*find_valid_path(char **cmd_path, char *total_cmd)
{
	char	*total_cmd_path;
	int		i;

	i = 0;
	total_cmd_path = NULL;
	while (cmd_path[i])
	{
		total_cmd_path = ft_strcat(cmd_path[i], total_cmd);
		if (!total_cmd_path)
			return (NULL);
		if (access(total_cmd_path, F_OK | X_OK) == 0)
		{
			return (total_cmd_path);
		}
		free(total_cmd_path);
		i ++;
	}
	return (NULL);
}

char	*check_access(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	else
		return (NULL);
}

char	*get_cmd_path(t_data *data, char *cmd)
{
	char	**cmd_path;
	char	*total_cmd;
	char	*valid_path;
	char	*env_path;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (check_access(cmd));
	env_path = find_variable_in_env(data, "PATH=");
	if (!env_path)
		return (NULL);
	cmd_path = ft_split(env_path, ':');
	free(env_path);
	if (!cmd_path)
		return (NULL);
	total_cmd = ft_strcat("/", cmd);
	if (!total_cmd)
		return (handle_invalid_cmd_path(cmd_path, total_cmd));
	valid_path = find_valid_path(cmd_path, total_cmd);
	if (!valid_path)
		return (handle_invalid_cmd_path(cmd_path, total_cmd));
	free_cmd_path(cmd_path, total_cmd);
	return (valid_path);
}
