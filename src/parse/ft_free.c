/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:02:07 by paromero          #+#    #+#             */
/*   Updated: 2025/04/02 19:24:23 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	free_while(t_data	*data)
{
	if (data->line)
	{
		free (data->line);
		data->line = NULL;
	}
	if (data->tokens)
	{
		ft_free_tokens(data->tokens);
		data->tokens = NULL;
	}
	if (data->ast)
	{
		ft_free_ast(data->ast);
		data->ast = NULL;
	}
	if (data->prompt)
	{
		free (data->prompt);
		data->prompt = NULL;
	}
	return (0);
}

void	free_minishell(t_data	*data)
{
	free_while(data);
	if (data->cwd)
		free (data->cwd);
	if (data->env)
		ft_free_env(data->env);
	rl_clear_history();
}
