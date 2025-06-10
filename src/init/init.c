/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:53:02 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 15:24:42 by paromero         ###   ########.fr       */
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

static void	update_shlvl_value(t_env *current)
{
	char	*new_value;
	int		shlvl;
	char	*shell_level;

	shlvl = ft_atoi(current->value + 6);
	shlvl++;
	shell_level = ft_itoa(shlvl);
	if (!shell_level)
		return ;
	new_value = ft_strcat("SHLVL=", shell_level);
	free(shell_level);
	if (!new_value)
		return ;
	free(current->value);
	current->value = new_value;
}

void	increment_shlvl(t_env *env)
{
	t_env	*current;

	if (!env)
		return ;
	current = env;
	while (current)
	{
		if (current->value && ft_strncmp(current->value, "SHLVL=", 6) == 0)
		{
			update_shlvl_value(current);
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

int	init_data(t_data *data, char **env)
{
	init_data_fields(data);
	rl_clear_history();
	if (!init_env(data, env))
	{
		printf("Error initializing env");
		free(data->env);
		return (0);
	}
	increment_shlvl(data->env);
	return (1);
}
