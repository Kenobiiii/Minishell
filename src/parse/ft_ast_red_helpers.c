/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_red_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:35:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/03 11:00:48 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Creates and handles the redirection node based on type
t_ast	*create_redirection_node(t_ast **cmd, t_ast **last_op,
		t_tokens *tokens)
{
	t_ast	*new_node;

	new_node = ft_create_ast_node(CMD, tokens->value);
	if (!new_node)
		return (NULL);
	if (is_redin2(last_op))
		redin2(cmd, last_op, new_node, tokens);
	else
		(*last_op)->right = new_node;
	return (new_node);
}

//? Cleans up unused redirection nodes
void	cleanup_unused_node(t_ast *new_node, t_ast **last_op)
{
	if (new_node && !is_redin2(last_op) && (*last_op)->right != new_node)
	{
		if (new_node->value)
			free(new_node->value);
		if (new_node->args)
			free_matrix(new_node->args);
		free(new_node);
	}
}

//? Finalizes the AST structure after redirection handling
void	finalize_redirection_ast(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_finalize_params *params)
{
	if (!(*last_op)->left && params->original_cmd)
		(*last_op)->left = params->original_cmd;
	if (!*root)
		*root = *last_op;
	*cmd = *last_op;
	*last_op = NULL;
}
