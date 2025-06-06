/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:27:38 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/31 12:45:31 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

void	exec_redirect_out(t_data *data, t_ast *node)
{
	execute_redirect_helper(data, node, O_WRONLY | O_CREAT | O_TRUNC);
}

void	exec_redirect_append(t_data *data, t_ast *node)
{
	execute_redirect_helper(data, node, O_WRONLY | O_CREAT | O_APPEND);
}

void	exec_redirect_in(t_data *data, t_ast *node)
{
	char	*filename;

	if (validate_input_redirect_node(data, node) == -1)
		return ;
	filename = node->right->value;
	if (handle_input_redirect_open(data, filename) == -1)
		return ;
	if (node->left)
		exec_ast(data, node->left);
}
