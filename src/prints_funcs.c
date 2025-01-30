/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:31:07 by paromero          #+#    #+#             */
/*   Updated: 2025/01/30 12:08:17 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printmatrix(char **matrix)
{
	int i = 0;

	while (matrix[i])
	{
		printf("token %d : %s -final\n", i, matrix[i]);
		i++;
	}
}

int	print_tokens(t_tokens *token) //! BORRAR AL ACABAR (TESTEO)
{
	while (token)
	{
		printf("Value: %s\n", token->value);
		printf("Type: ");
		print_type(token); // Llama a print_type para imprimir el tipo
		token = token->next;
	}
	return (1);
}

void	print_type(t_tokens *token) //! BORRAR AL ACABAR (TESTEO)
{
	if (token->type == CMD)
		printf("CMD\n");
	else if (token->type == REDIRECT_OUT)
		printf("REDIRECT_OUT\n");
	else if (token->type == REDIRECT_IN)
		printf("REDIRECT_IN\n");
	else if (token->type == REDIN2)
		printf("REDIN2\n");
	else if (token->type == REDOUT2)
		printf("REDOUT2\n");
	else if (token->type == PIPE)
		printf("PIPE\n");
	else if (token->type == AND)
		printf("AND\n");
	else if (token->type == OR)
		printf("OR\n");
	else
		printf("UNKNOWN TYPE\n");
}

void print_ast(t_ast *node, int level) //! BORRAR AL ACABAR (TESTEO)
{
	if (!node)
		return;

	// Indentación para mostrar la profundidad del nodo
	for (int i = 0; i < level; i++)
		printf("  ");

	// Imprimir el tipo y el valor del nodo
	if (node->type == CMD)
		printf("CMD: %s\n", node->value);
	else
		printf("OPERATOR: %s\n", node->value);

	// Imprimir los argumentos si es un nodo de comando
	if (node->type == CMD && node->args)
	{
		for (int i = 0; node->args[i]; i++)
		{
			for (int j = 0; j < level + 1; j++)
				printf("  ");
			printf("ARG: %s\n", node->args[i]);
		}
	}

	// Recursivamente imprimir los nodos hijos
	print_ast(node->left, level + 1);
	print_ast(node->right, level + 1);
}
