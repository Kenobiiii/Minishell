/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_and_or.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:26:39 by anggalle          #+#    #+#             */
/*   Updated: 2025/02/17 19:27:05 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_logical_or(t_data *data, t_ast *node)
{
	exec_ast(data, node->left);
	if (WEXITSTATUS(data->wstatus) != 0)
	{
		exec_ast(data, node->right);
	}
}

void	exec_logical_and(t_data *data, t_ast *node)
{
	exec_ast(data, node->left);
	if (WEXITSTATUS(data->wstatus) == 0)
	{
		exec_ast(data, node->right);
	}
}
