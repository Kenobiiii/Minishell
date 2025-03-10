/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_and_or.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:26:39 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/10 12:06:58 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_logical_or(t_data *data, t_ast *node)
{
	exec_ast(data, node->left);
	if (data->wstatus != 0)
	{
		exec_ast(data, node->right);
	}
}

void	exec_logical_and(t_data *data, t_ast *node)
{
	exec_ast(data, node->left);
	if (data->wstatus == 0)
	{
		exec_ast(data, node->right);
	}
}
