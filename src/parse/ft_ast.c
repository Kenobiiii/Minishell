/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:30 by paromero          #+#    #+#             */
/*   Updated: 2025/05/04 13:46:09 by paromero         ###   ########.fr       */
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

static int ft_handle_operator_node(t_data *data, t_ast **root, t_ast **cmd,
	t_ast **last_op, t_tokens *tokens)
{
	t_ast *new_op;
	t_ast *current;
	int found_same_redir = 0;
	char  *filename;
	int   fd;


	// --- Perform File Check BEFORE modifying AST ---
	if (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT ||
		tokens->type == REDIN2 || tokens->type == REDOUT2)
	{
		// Assuming the filename is stored in tokens->value for redirection tokens
		filename = tokens->next->value;
		if (!filename || !*filename) // Check for NULL or empty filename
		{
			data->wstatus = 1;
			return (0); // Failure
		}

		if (tokens->type == REDIRECT_IN) // Check read access for '<'
		{
			if (access(filename, R_OK) == -1)
			{
				// Solo registrar el error pero CONTINUAR con el parsing
				data->wstatus = 1;
				// NO retornar 0 aquí
			}
		}
		else if (tokens->type == REDIRECT_OUT || tokens->type == REDOUT2) // Check write/create access for '>' or '>>'
		{
			int flags = (tokens->type == REDOUT2) ?
						(O_WRONLY | O_CREAT | O_APPEND) :
						(O_WRONLY | O_CREAT | O_TRUNC);
			fd = open(filename, flags, 0644);
			if (fd == -1)
			{
				data->wstatus = 1;
				return (0); // Failure
			}
			close(fd); // Successfully opened (and created if needed), close it.
		}
		// Note: REDIN2 ('<<') does not involve a file check at this stage.
	}
	// --- End File Check ---


	// Solo buscar redirecciones duplicadas si es un tipo de redirección
	if (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT ||
		tokens->type == REDIN2 || tokens->type == REDOUT2)
	{
		// Buscar redirecciones desde la raíz del comando actual
		current = *root;
		while (current)
		{
			// Si encontramos una redirección del mismo tipo
			if (current->type == tokens->type)
			{
				found_same_redir = 1;
				
				// Actualizar el archivo (valor a la derecha)
				if (current->right)
				{
					free(current->right->value);
					current->right->value = ft_strdup(tokens->value);
					if (!current->right->value) return (0); // Malloc check
				}
				else
				{
					// Si no hay nodo derecho, crearlo
					current->right = ft_create_ast_node(CMD, tokens->value);
					if (!current->right || !current->right->value) return (0); // Malloc check
				}
				*last_op = current; // Mantener el operador actual como último operador
				break;
			}
			
			// Solo buscar en la rama principal del árbol
			if (current->right && (current->type == REDIRECT_IN || 
								current->type == REDIRECT_OUT ||
								current->type == REDIN2 || 
								current->type == REDOUT2))
				current = current->right;
			else
				break;
		}
	}

	// Si no encontramos una redirección del mismo tipo, crear nuevo nodo
	if (!found_same_redir)
	{
		new_op = ft_create_ast_node(tokens->type, tokens->value);
		if (!new_op)
			return (0); // Malloc check
			
		if (*cmd)
			connect_operator(root, cmd, last_op, new_op);
		else
		{
			*root = new_op;
			*last_op = new_op;
		}
	}
	return (1);
}

// static void print_ast(t_ast *node, int level) //! BORRAR AL ACABAR (TESTEO)
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


t_ast	*ft_build_ast(t_data *data, t_tokens *tokens)
{
	t_ast	*root;
	t_ast	*cmd;
	t_ast	*last_op;
	int     parse_success;

	parse_success = 1;
	root = NULL;
	cmd = NULL;
	last_op = NULL;
	while (tokens && parse_success)
	{
		if (tokens->type == CMD)
			ft_handle_command_node(&root, &cmd, &last_op, tokens);
		else
			parse_success = ft_handle_operator_node(data, &root, &cmd, &last_op, tokens);
		if (parse_success)
			tokens = tokens->next;
	}
	if (!parse_success)
	{
		ft_free_ast(root);
		return (NULL);
	}
	// ft_printf("---- AST FINAL ----\n");
	// print_ast(root, 0);  // Necesitarás implementar esta función si no existe
	return (root);
}
