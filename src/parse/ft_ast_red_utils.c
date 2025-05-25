/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_red_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:13:17 by paromero          #+#    #+#             */
/*   Updated: 2025/05/25 17:43:12 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Verifica si el token es de redirección
int	is_redirect_token(t_tokens *tokens)
{
	return (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT
		|| tokens->type == REDIN2 || tokens->type == REDOUT2);
}

//? Verifica si el operador es un operador de redirección
int	is_red(t_ast **last_op)
{
	if (last_op && *last_op && ((*last_op)->type == REDIN2
			|| (*last_op)->type == REDOUT2
			|| (*last_op)->type == REDIRECT_IN
			|| (*last_op)->type == REDIRECT_OUT))
		return (1);
	return (0);
}

//? Maneja redirecciones en operadores
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

//? Maneja la creación de nodos de redirección y su conexión con comandos
void	handle_redirection(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*original_cmd;
	t_ast	*new_node;

	original_cmd = *cmd;
	if (is_redin2(last_op))
	{
		new_node = ft_create_ast_node(CMD, tokens->value);
		if (new_node)
			redin2(cmd, last_op, new_node, tokens);
	}
	else
	{
		(*last_op)->right = ft_create_ast_node(CMD, tokens->value);
	}
	if (!(*last_op)->left && original_cmd)
		(*last_op)->left = original_cmd;
	if (!*root)
		*root = *last_op;
	*cmd = *last_op;
	*last_op = NULL;
}
