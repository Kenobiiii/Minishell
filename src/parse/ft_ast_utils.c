/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:26:41 by paromero          #+#    #+#             */
/*   Updated: 2025/06/05 18:11:24 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Creates a new node for the AST with the specified type and value
t_ast	*ft_create_ast_node(t_type type, char *value)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node);
		return (NULL);
	}
	node->args = NULL;
	node->right = NULL;
	node->left = NULL;
	return (node);
}

//? Connects a new operator with the current command node 
//? and updates the AST root
void	connect_operator_to_ast(t_ast **root, t_ast **cmd,
	t_ast **last_op, t_ast *new_op)
{
	new_op->left = *cmd;
	if (*last_op)
		(*last_op)->right = new_op;
	else
		*root = new_op;
	*last_op = new_op;
	if (!is_redirection_operator(last_op))
		*cmd = NULL;
}

//? Creates a new operator node and connects it to the AST
void	handle_new_node(t_ast **root, t_ast **cmd,
	t_ast **last_op, t_ast *new_op)
{
	if (!new_op)
		return ;
	if (*cmd)
	{
		connect_operator_to_ast(root, cmd, last_op, new_op);
	}
	else
	{
		new_op->left = *root;
		*root = new_op;
		*last_op = new_op;
	}
}

//? Initializes the AST variables
void	init_ast_vars(t_ast **root, t_ast **cmd, t_ast **last_op,
	t_ast_args *args)
{
	*root = NULL;
	*cmd = NULL;
	*last_op = NULL;
	args->root = root;
	args->cmd = cmd;
	args->last_op = last_op;
}

//? Processes an individual token
int	process_token(t_data *data, t_ast_args *args)
{
	if (args->tokens->type == CMD)
	{
		ft_handle_command_node(args->root, args->cmd,
			args->last_op, args->tokens);
		return (1);
	}
	else
		return (ft_handle_operator_node(data, args));
}
