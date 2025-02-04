/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 17:54:00 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_sigint_received = 0;

int	line_syntax(t_data	*data)
{
	add_history(data->line);
	if (!ft_isspace(data->line))
		return (0);
	if (!openquotes(data->line))
	{
		printf("Syntax error\n");
		return (0);
	}
	if (!handle_invslash_pcomma(data->line))
		return (0);
	ft_tokens(data, data->line);
	data->ast = ft_build_ast(data->tokens);
	return (1);
}

int	minishell(char **env)
{
	t_data	data;

	init_data(&data, env);
	setup_signals(); //- Configurar señales al inicio
	while (data.exit == 0)
	{
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			data.line = NULL;
		}
		update_pwd(&data);
		data.line = readline(data.prompt);
		if (data.line == NULL)
			break ;
		if (!line_syntax(&data))
			continue ;
		if (is_builtins(&data) == 0)
			exec_func(&data);
		free_innerwhile(&data);
		continue ;
	}
	free_minishell(&data);
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
