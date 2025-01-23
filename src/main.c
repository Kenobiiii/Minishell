/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/01/23 11:18:12 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_isSpace(char	*line)
{
	int	i;

	i = 0;
	if (ft_strlen(line) == 0)
		return (0);
	while (line[i])
	{
		if (line[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}


int	minishell(char **env)
{
	t_data	data;

	init_data(&data, env);
	while (data.exit == 0)
	{
		//TODO Signal handler (Importante!!!!)
		data.line = readline(data.prompt);
		if (data.line == NULL)
			break ;
		if (!ft_isSpace(data.line))
			continue ;
		add_history(data.line);
		if (ft_strncmp(data.line, "exit", 5) == 0) //TODO quitarlo
			data.exit = 1;
		if (!opencoms(data.line))
		{
			printf("Syntax error\n");
			continue ;
		}
		data.line = ft_delete_spaces(data.line);
		ft_tokens(&data, data.line);
		if (data.line && ft_syntax(&data))
		{
			//print_tokens(data.tokens);
			//TODO func parseo
			data.ast = ft_build_ast(data.tokens);
			//print_ast(data.ast, 1);

			//TODO func ejecutable
			if (is_builtins(&data) == 1)
			{
				continue;
			}
			exec_func(&data);
			continue;
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
