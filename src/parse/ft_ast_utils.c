/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:26:41 by paromero          #+#    #+#             */
/*   Updated: 2025/04/08 18:34:20 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_red(t_ast **last_op)
{
	if (*last_op && ((*last_op)->type == REDIN2 || (*last_op)->type == REDOUT2
			|| (*last_op)->type == REDIRECT_IN
			|| (*last_op)->type == REDIRECT_OUT))
		return (1);
	return (0);
}

void	handle_redirection(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*original_cmd;

	original_cmd = *cmd;
	if (is_redin2(last_op))
		redin2(cmd, last_op, NULL, tokens);
	else
		(*last_op)->right = ft_create_ast_node(CMD, tokens->value);
	if (!(*last_op)->left && original_cmd)
		(*last_op)->left = original_cmd;
	if (!*root)
		*root = *last_op;
	*cmd = *last_op;
	*last_op = NULL;
}

void	connect_operator(t_ast **root, t_ast **cmd,
	t_ast **last_op, t_ast *new_op)
{
	new_op->left = *cmd;
	if (*last_op)
		(*last_op)->right = new_op;
	else
		*root = new_op;
	*last_op = new_op;
	if (!is_red(last_op))
		*cmd = NULL;
}
