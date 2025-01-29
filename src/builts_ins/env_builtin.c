/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:03:42 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/29 18:10:40 by anggalle         ###   ########.fr       */
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
	if (!data->ast->args[1])
	{
		print_env(data->env);
	}
	return (1);
}
