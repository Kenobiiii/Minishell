/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/05/27 19:26:06 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Definición de la ÚNICA variable global para el estado de las señales
volatile sig_atomic_t	g_shell_state = STATE_PROMPT_NORMAL;

int	minishell(char **env)
{
	t_data	data;
	int		result;

	init_data(&data, env);
	setup_signals();
	while (data.exit == 0)
	{
		if (handle_signal_states(&data))
			continue ;
		update_pwd(&data);
		data.line = readline(data.prompt);
		result = handle_readline_result(&data);
		if (result == -1)
			break ;
		if (result == 0)
			continue ;
		process_command_line(&data);
		free_while(&data);
	}
	free_minishell(&data);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	if (ac != 1)
	{
		printf("Error: More than one arguments\n");
		return (0);
	}
	(void)ac;
	(void)av;
	minishell(env);
	return (0);
}
