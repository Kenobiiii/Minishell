/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_red_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:13:17 by paromero          #+#    #+#             */
/*   Updated: 2025/06/03 10:31:47 by paromero         ###   ########.fr       */
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
	t_ast				*new_node;
	t_finalize_params	params;

	if (!last_op || !*last_op)
		return ;
	params.logical_op_to_preserve = find_logical_op_to_preserve(*root);
	params.original_cmd = *cmd;
	new_node = create_redirection_node(cmd, last_op, tokens);
	cleanup_unused_node(new_node, last_op);
	finalize_redirection_ast(root, cmd, last_op, &params);
}
