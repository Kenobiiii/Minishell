/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:30 by paromero          #+#    #+#             */
/*   Updated: 2025/01/09 19:39:53 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*ft_create_ast_node(t_type type, char *value)
{
	t_ast	*node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	node->args = NULL;
	node->right = NULL;
	node->left = NULL;
	return (node);
}

t_ast	*ft_build_ast(t_tokens *tokens)
{
	t_ast	*root;
	t_ast	*current_cmd;
	t_ast	*new_node;

	root = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		if (tokens->type == CMD)
		{
			// Crear un nuevo nodo para el comando
			new_node = ft_create_ast_node(tokens->value, CMD);
			// Si hay un comando actual, agregar el argumento
			if (current_cmd)
				ft_add_argument(current_cmd, tokens->value);
			else
			{
				// Primer comando, establecer como nodo actual
				current_cmd = new_node;
				if (!root)
					root = current_cmd;
			}
		}
		else
		{
			// Crear nodos para operadores y conectar
			new_node = ft_create_ast_node(tokens->value, tokens->type);
			new_node->left = root;  // El operador conecta el árbol existente
			root = new_node;
			current_cmd = NULL; // Resetear para el siguiente comando
		}
		tokens = tokens->next;
	}
	return root;
}

void	ft_add_argument(t_ast *cmd_node, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	while (cmd_node->args && cmd_node->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return; // Manejar error de memoria si es necesario
	while (i--)
		new_args[i] = cmd_node->args[i];
	new_args[i + 1] = ft_strdup(arg);
	new_args[i + 2] = NULL;
	free(cmd_node->args);
	cmd_node->args = new_args;
}
