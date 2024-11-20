/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:53:02 by paromero          #+#    #+#             */
/*   Updated: 2024/11/20 18:36:55 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * TODO guardar SHLVL + 1
 */

int	print_env(t_env *env) //! Borrar trás pruebas (guardar más bien)
{
	while (env)
	{
		printf("%s\n", env->value);
		env = env->next;
	}
	return (1);
}

t_env	*create_node(const char *valor)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(valor);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = (NULL);
	return (new_node);
}

int	init_env(t_data *data, char *env[])
{
	t_env	*current;
	int		i;

	data->env = create_node(env[0]);
	if (!data->env)
		return (0);
	current = data->env;
	i = 1;
	while (env[i])
	{
		current->next = create_node(env[i]);
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

	data->env = create_node(ft_strcat("PWD=", data->cwd));
	if (!data->env)
		return (0);
	current = data->env;
	current->next = create_node("SHLVL=1");
		if (!current->next)
			return (0);
		current = current->next;
	current->next = create_node("_=/usr/bin/env");
		if (!current->next)
			return (0);
		current = current->next;
	return (1);
}

int	init_data(t_data *data, char **env)
{
	char	cwd[PATH_MAX];

	data->line = NULL;
	data->last_exit_status = 0;
	data->signal_received = 0;
	data->pid = -1;
	data->prompt = "$Minishell> ";
	data->cwd = ft_strdup(getcwd(cwd, sizeof(cwd)));
	data->tokens = NULL;
	data->ast = NULL;
	rl_clear_history();
	if (env == NULL || env[0] == NULL)
	{
		init_empty_env(data);
	}
	else
	{
		if (!init_env(data, env))
		{
			printf("Error initializing env");
			free(data->env);
			return (0);
		}
	}
	if (!print_env(data->env)) //! Borrar trás pruebas (guardar más bien)
	{
		printf("Error printing env\n");
		return (0);
	}
	return (1);
}

/**
 * TODO Mensaje de error custom, depende de lo que le pase salte un error u otro
 */

