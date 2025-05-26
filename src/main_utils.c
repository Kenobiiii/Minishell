/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:00:00 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/25 21:55:43 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_shell_state;

static void	reset_line_and_state(t_data *data)
{
	data->wstatus = 130;
	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	g_shell_state = STATE_PROMPT_NORMAL;
}

int	handle_signal_states(t_data *data)
{
	if (g_shell_state == STATE_PROMPT_INTERRUPTED)
		reset_line_and_state(data);
	else if (g_shell_state == STATE_EXECUTION_INTERRUPTED)
	{
		reset_line_and_state(data);
		return (1);
	}
	else if (g_shell_state == STATE_EXECUTING)
		g_shell_state = STATE_PROMPT_NORMAL;
	if (g_shell_state != STATE_PROMPT_NORMAL)
		g_shell_state = STATE_PROMPT_NORMAL;
	return (0);
}

int	handle_readline_result(t_data *data)
{
	if (data->line == NULL && g_shell_state == STATE_PROMPT_INTERRUPTED)
		return (0);
	if (data->line == NULL)
	{
		printf("exit\n");
		return (-1);
	}
	return (1);
}

void	process_command_line(t_data *data)
{
	if (data->line && *data->line)
	{
		if (!line_syntax(data))
		{
			free_while(data);
			return ;
		}
		g_shell_state = STATE_EXECUTING;
		if (is_builtins(data, data->ast->value) == 0)
			exec_func(data);
	}
}
