/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/01/14 18:51:08 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_ast(t_ast *node, int level)
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

int	minishell(char **env)
{
	t_data	data;
	t_ast	*root = NULL;

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
			root = ft_build_ast(data.tokens);
			print_ast(root, 1);

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
