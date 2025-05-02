/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:30 by paromero          #+#    #+#             */
/*   Updated: 2025/05/02 11:22:32 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	handle_new_command(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*new_cmd;

	new_cmd = ft_create_ast_node(CMD, tokens->value);
	if (!new_cmd)
		return ;
	new_cmd->args = malloc(sizeof(char *) * 2);
	if (new_cmd->args)
	{
		new_cmd->args[0] = ft_strdup(tokens->value);
		new_cmd->args[1] = NULL;
	}
	*cmd = new_cmd;
	if (!*root)
		*root = *cmd;
	else if (*last_op)
		(*last_op)->right = *cmd;
}

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
		if (cmd_redir)
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

void ft_handle_operator_node(t_ast **root, t_ast **cmd,
	t_ast **last_op, t_tokens *tokens)
{
	t_ast *new_op;
	t_ast *current;
	int found_same_redir;

	found_same_redir = 0;
	if (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT ||
		tokens->type == REDIN2 || tokens->type == REDOUT2)
	{
		current = *root;
		while (current)
		{
			if (current->type == tokens->type)
			{
				found_same_redir = 1;
				if (current->right)
				{
					free(current->right->value);
					current->right->value = ft_strdup(tokens->value);
				}
				break;
			}
			current = current->right;
		}
	}

	// Si encontramos una redirección del mismo tipo, no necesitamos crear un nuevo nodo
	if (found_same_redir)
		return;

	// Código normal para crear y conectar el nuevo operador
	new_op = ft_create_ast_node(tokens->type, tokens->value);
	if (!new_op)
		return;
	if (*cmd)
		connect_operator(root, cmd, last_op, new_op);
	else
	{
		*root = new_op;
		*last_op = new_op;
	}
}

// void print_ast(t_ast *node, int level) //! BORRAR AL ACABAR (TESTEO)
// {
// 	if (!node)
// 		return;

// 	// Indentación para mostrar la profundidad del nodo
// 	for (int i = 0; i < level; i++)
// 		printf("  ");

// 	// Imprimir el tipo y el valor del nodo
// 	if (node->type == CMD)
// 		printf("CMD: %s\n", node->value);
// 	else
// 		printf("OPERATOR: %s\n", node->value);

// 	// Imprimir los argumentos si es un nodo de comando
// 	if (node->type == CMD && node->args)
// 	{
// 		for (int i = 0; node->args[i]; i++)
// 		{
// 			for (int j = 0; j < level + 1; j++)
// 				printf("  ");
// 			printf("ARG: %s\n", node->args[i]);
// 		}
// 	}

// 	// Recursivamente imprimir los nodos hijos
// 	print_ast(node->left, level + 1);
// 	print_ast(node->right, level + 1);
// }


t_ast	*ft_build_ast(t_tokens *tokens)
{
	t_ast	*root;
	t_ast	*cmd;
	t_ast	*last_op;

	root = NULL;
	cmd = NULL;
	last_op = NULL;
	while (tokens)
	{
		if (tokens->type == CMD)
			ft_handle_command_node(&root, &cmd, &last_op, tokens);
		else
			ft_handle_operator_node(&root, &cmd, &last_op, tokens);
		tokens = tokens->next;
	}
	// En ft_build_ast, justo antes del return
	// ft_printf("---- AST FINAL ----\n");
	// print_ast(root, 0);  // Necesitarás implementar esta función si no existe
	return (root);
}
