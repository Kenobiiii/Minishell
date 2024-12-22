/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2024/12/10 18:15:14 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_type(t_tokens *token)
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

int	print_tokens(t_tokens *token)
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
void	print_ast(t_ast *node, int level)
{
	int i;

	if (!node)
		return;

	// Indentar según el nivel en el árbol
	for (i = 0; i < level; i++)
		printf("  ");

	// Imprimir el tipo del nodo
	if (node->type == CMD)
		printf("Node Type: CMD, Value: %s, Args: ", node->value);
	else if (node->type == PIPE)
		printf("Node Type: PIPE\n");
	else if (node->type == REDIRECT_IN)
		printf("Node Type: REDIRECT_IN, File: %s\n", node->value);
	else if (node->type == REDIRECT_OUT)
		printf("Node Type: REDIRECT_OUT, File: %s\n", node->value);
	else if (node->type == REDIN2)
		printf("Node Type: REDIN2, File: %s\n", node->value);
	else if (node->type == REDOUT2)
		printf("Node Type: REDOUT2, File: %s\n", node->value);
	else if (node->type == AND)
		printf("Node Type: AND\n");
	else if (node->type == OR)
		printf("Node Type: OR\n");
	else
		printf("Node Type: UNKNOWN\n");

	// Imprimir los argumentos si el nodo es un comando
	if (node->type == CMD && node->args)
	{
		for (i = 0; node->args[i]; i++)
		{
			if (i > 0)
				printf(", ");
			printf("%s", node->args[i]);
		}
		printf("\n");
	}

	// Recursivamente imprimir el subárbol izquierdo y derecho
	print_ast(node->left, level + 1);
	print_ast(node->right, level + 1);
}


int	minishell(char **env)
{
	t_data	data;

	init_data(&data, env);
	while (data.exit == 0)
	{
		//TODO Signal handler (Importante!!!!)
		configure_signals();
		data.line = readline(data.prompt);
		if (data.line == NULL)
			break ;
		if (ft_strlen(data.line) == 0)
			continue ;
		add_history(data.line);
		if (ft_strncmp(data.line, "exit", 5) == 0)
			data.exit = 1;
		data.line = ft_spaces(data.line);
		ft_tokens(&data, data.line);
		if (data.line && ft_syntax(&data))
		{
			//print_tokens(data.tokens);
			//TODO func parseo
			//ft_ast(&data);
			print_ast(data.ast, 0);

			//TODO func ejecutable
		}
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	if (ac != 1)
	{
		printf("Error: More than one argument");
		return (0);
	}
	(void)ac;
	(void)av;
	minishell(env);
	return (0);
}
