/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_red_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:13:17 by paromero          #+#    #+#             */
/*   Updated: 2025/05/31 10:02:36 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Checks if the token is a redirection token
int	is_redirect_token(t_tokens *tokens)
{
	return (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT
		|| tokens->type == REDIN2 || tokens->type == REDOUT2);
}

//? Checks if the operator is a redirection operator
int	is_red(t_ast **last_op)
{
	if (last_op && *last_op && ((*last_op)->type == REDIN2
			|| (*last_op)->type == REDOUT2
			|| (*last_op)->type == REDIRECT_IN
			|| (*last_op)->type == REDIRECT_OUT))
		return (1);
	return (0);
}

//? Handles redirections in operators
int	handle_redirect_operator(t_ast_args *args)
{
	t_ast	*new_op;
	int		found_same_redir;

	found_same_redir = handle_redirect_node(args->root,
			args->last_op, args->tokens);
	if (!found_same_redir)
	{
		new_op = ft_create_ast_node(args->tokens->type,
				args->tokens->value);
		if (!new_op)
			return (0);
		handle_new_node(args->root, args->cmd, args->last_op, new_op);
	}
	return (1);
}

//? Handles creation of redirection nodes and their connection with commands
void	handle_redirection(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*original_cmd;
	t_ast	*new_node;
	t_ast	*logical_op_to_preserve;
	t_ast	*current;

	if (!last_op || !*last_op)
		return ;
	logical_op_to_preserve = NULL;
	if (*root)
	{
		current = *root;
		while (current)
		{
			if (current->type == AND || current->type == OR)
			{
				logical_op_to_preserve = current;
				break ;
			}
			if (current->right && (current->right->type == AND
					|| current->right->type == OR))
			{
				logical_op_to_preserve = current->right;
				break ;
			}
			current = current->right;
		}
	}
	original_cmd = *cmd;
	new_node = NULL;
	if (is_redin2(last_op))
	{
		new_node = ft_create_ast_node(CMD, tokens->value);
		if (new_node)
			redin2(cmd, last_op, new_node, tokens);
	}
	else
	{
		new_node = ft_create_ast_node(CMD, tokens->value);
		if (new_node)
			(*last_op)->right = new_node;
	}
	if (new_node && !is_redin2(last_op) && (*last_op)->right != new_node)
	{
		if (new_node->value)
			free(new_node->value);
		if (new_node->args)
			free_matrix(new_node->args);
		free(new_node);
	}
	if (!(*last_op)->left && original_cmd)
		(*last_op)->left = original_cmd;
	if (!*root)
		*root = *last_op;
	*cmd = *last_op;
	if (logical_op_to_preserve && logical_op_to_preserve != *last_op)
		*last_op = logical_op_to_preserve;
	else
		*last_op = NULL;
}
