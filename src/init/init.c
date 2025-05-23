/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:53:02 by paromero          #+#    #+#             */
/*   Updated: 2025/05/06 18:36:06 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*create_env_node(const char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = (NULL);
	return (new_node);
}

void	increment_shlvl(t_env *env)
{
	t_env	*current;
	char	*new_value;
	int		shlvl;
	char	*shell_level;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->value, "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi(current->value + 6);
			shlvl++;
			shell_level = ft_itoa(shlvl);
			new_value = ft_strcat("SHLVL=", shell_level);
			free(shell_level);
			free(current->value);
			current->value = new_value;
			return ;
		}
		current = current->next;
	}
}

int	init_env(t_data *data, char *env[])
{
	t_env	*current;
	int		i;

	data->env = create_env_node(env[0]);
	if (!data->env)
		return (0);
	current = data->env;
	i = 1;
	while (env[i])
	{
		current->next = create_env_node(env[i]);
		if (!current->next)
			return (0);
		current = current->next;
		i++;
	}
	return (1);
}

int	init_empty_env(t_data *data)
{
	t_env	*current;

	data->env = create_env_node(ft_strcat("PWD=", data->cwd));
	if (!data->env)
		return (0);
	current = data->env;
	current->next = create_env_node("SHLVL=1");
	if (!current->next)
		return (0);
	current = current->next;
	current->next = create_env_node("_=/usr/bin/env");
	if (!current->next)
		return (0);
	current = current->next;
	return (1);
}

int	init_data(t_data *data, char **env)
{
	char	cwd[PATH_MAX];

	data->line = NULL;
	data->exit = 0;
	data->last_exit_status = 0;
	data->pid = -1;
	data->prompt = "$Minishell> ";
	data->cwd = ft_strdup(getcwd(cwd, sizeof(cwd)));
	data->tokens = NULL;
	data->ast = NULL;
	data->only_redirections = 1;
	data->wstatus = 0;
	rl_clear_history();
	if (env == NULL || env[0] == NULL)
		init_empty_env(data);
	else
	{
		if (!init_env(data, env))
		{
			printf("Error initializing env");
			free(data->env);
			return (0);
		}
		increment_shlvl(data->env);
	}
	return (1);
}

/**
 * TODO Mensaje de error custom, depende de lo que le pase salte un error u otro
 */