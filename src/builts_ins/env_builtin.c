/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:03:42 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/01 16:54:48 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_env(t_env *env)
{
	while (env)
	{
		printf("%s\n", env->value);
		env = env->next;
	}
	return (1);
}

int	env_builtin(t_data *data)
{
	if (!data->ast->args[2])
		printf("Too many arguments\n");
	else
		print_env(data->env);
	return (1);
}
