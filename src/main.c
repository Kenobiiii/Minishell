/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2024/11/28 19:25:27 by paromero         ###   ########.fr       */
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

int	minishell(char **env)
{
	t_data data;

	init_data(&data, env);
	while (data.exit == 0)
	{
		//TODO Signal handler (Importante!!!!)
		data.line = readline(data.prompt);
		if (data.line == NULL)
			break ;
		if (ft_strlen(data.line) == 0)
			continue ;
		add_history(data.line);
		if (ft_strncmp(data.line, "exit", 5) == 0)
			data.exit = 1;
		if (data.line) //TODO && sintax_handler)
		{
			data.line = ft_spaces(data.line);
			ft_tokens(&data, data.line);
			print_tokens(data.tokens);
			//TODO func parseo
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
