/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:53:02 by paromero          #+#    #+#             */
/*   Updated: 2024/11/19 17:30:58 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	if (env == NULL || env[0] == NULL)
	{
		printf("El entorno está vacío.\n");
		data->env_head = NULL;
		return (1);
	}
	data->env_head = create_node(env[0]);
	if (!data->env_head)
		return (0);
	current = data->env_head;
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

int	init_data(t_data *data, char **env)
{
	data->line = NULL;
	data->last_exit_status = 0;
	data->signal_received = 0;
	data->pid = -1;
	data->prompt = "$Minishell> ";
	data->cwd = getcwd;
	data->tokens = NULL;
	data->ast = NULL;
}
