/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:02:07 by paromero          #+#    #+#             */
/*   Updated: 2025/01/29 20:54:11 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_innerwhile(t_data  *data)
{
	free (data->line);
	ft_free_tokens(data->tokens);
	data->tokens = NULL;
	ft_free_ast(data->ast);
	data->ast = NULL;
}

void	free_minishell(t_data  *data)
{
	free (data->cwd);
	ft_free_env(data->env);
	rl_clear_history();
}