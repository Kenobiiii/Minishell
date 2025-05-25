/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:26:41 by paromero          #+#    #+#             */
/*   Updated: 2025/05/25 17:27:13 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Crea un nuevo nodo para el AST con el tipo y valor especificados
t_ast	*ft_create_ast_node(t_type type, char *value)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	node->args = NULL;
	node->right = NULL;
	node->left = NULL;
	return (node);
}

//? Conecta un nuevo operador con el nodo de comando actual 
//? y actualiza la raíz del AST
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

//? Crea un nuevo nodo operador y lo conecta al AST
void	handle_new_node(t_ast **root, t_ast **cmd,
	t_ast **last_op, t_ast *new_op)
{
	if (*cmd)
		connect_operator(root, cmd, last_op, new_op);
	else
	{
		*root = new_op;
		*last_op = new_op;
	}
}

//? Inicializa las variables del AST
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

//? Procesa un token individual
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
