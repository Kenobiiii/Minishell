/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:30 by paromero          #+#    #+#             */
/*   Updated: 2025/05/30 20:45:01 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Creates a new command node and connects it to the AST if necessary
static	void	handle_new_command(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*new_cmd;

	new_cmd = ft_create_ast_node(CMD, tokens->value);
	if (!new_cmd)
		return ;
	new_cmd->args = malloc(sizeof(char *) * 2);
	if (!new_cmd->args)
	{
		ft_free_command_node(new_cmd, 0);
		return ;
	}
	new_cmd->args[0] = ft_strdup(tokens->value);
	if (!new_cmd->args[0])
	{
		ft_free_command_node(new_cmd, 1);
		return ;
	}
	new_cmd->args[1] = NULL;
	*cmd = new_cmd;
	if (!*root)
		*root = *cmd;
	else if (*last_op)
		(*last_op)->right = *cmd;
}

//? Processes a command token and adds it to the AST
void	ft_handle_command_node(t_ast **root, t_ast **cmd,
	t_ast **last_op, t_tokens *tokens)
{
	t_ast	*cmd_redir;

	cmd_redir = NULL;
	if (is_red(last_op))
	{
		if (*cmd && (*cmd)->type == CMD)
			cmd_redir = *cmd;
		handle_redirection(root, cmd, last_op, tokens);
		if (cmd_redir && *cmd && !(*cmd)->left)
			(*cmd)->left = cmd_redir;
	}
	else if (*cmd)
	{
		if ((*cmd)->type == CMD)
			ft_add_argument(*cmd, tokens->value);
		else if ((*cmd)->left && (*cmd)->left->type == CMD)
			ft_add_argument((*cmd)->left, tokens->value);
		else
			handle_command_as_arg(cmd, cmd, tokens);
	}
	else
		handle_new_command(root, cmd, last_op, tokens);
}

//? Processes an operator token (pipe, redirection, &&, ||)
//? and adds it to the AST
int	ft_handle_operator_node(t_data *data, t_ast_args *args)
{
	t_ast	*new_op;

	if (!handle_redirect_checks(data, args->tokens))
		return (0);
	if (is_redirect_token(args->tokens))
		return (handle_redirect_operator(args));
	else
	{
		new_op = ft_create_ast_node(args->tokens->type, args->tokens->value);
		if (!new_op)
			return (0);
		handle_new_node(args->root, args->cmd, args->last_op, new_op);
		
		// Verificar que el nodo se conectó correctamente al AST
		if (args->root && *args->root && new_op != *args->root && 
			(!args->last_op || *args->last_op != new_op))
		{
			// Si el nodo no se conectó correctamente, liberarlo
			if (new_op->value)
				free(new_op->value);
			if (new_op->args)
				free_matrix(new_op->args);
			free(new_op);
			return (0);
		}
	}
	return (1);
}

//? Construye el AST a partir de la lista de tokens
t_ast	*ft_build_ast(t_data *data, t_tokens *tokens)
{
	t_ast		*root;
	t_ast		*cmd;
	t_ast		*last_op;
	int			parse_success;
	t_ast_args	args;

	parse_success = 1;
	init_ast_vars(&root, &cmd, &last_op, &args);
	args.tokens = tokens;
	while (tokens && parse_success)
	{
		args.tokens = tokens;
		parse_success = process_token(data, &args);
		if (parse_success)
			tokens = tokens->next;
	}
	if (!parse_success)
	{
		ft_free_ast(root);
		return (NULL);
	}
	return (root);
}
