/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:00:00 by anggalle          #+#    #+#             */
/*   Updated: 2025/06/10 14:20:47 by paromero         ###   ########.fr       */
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
	setup_signals();
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
	{
		g_shell_state = STATE_PROMPT_NORMAL;
		setup_signals();
	}
	if (g_shell_state != STATE_PROMPT_NORMAL)
	{
		g_shell_state = STATE_PROMPT_NORMAL;
		setup_signals();
	}
	return (0);
}

int	handle_readline_result(t_data *data)
{
	if (data->line == NULL)
	{
		printf("exit\n");
		return (-1);
	}
	return (1);
}

int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	execute_builtin_with_redirections(t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;
	int	builtin_result;

	if (!data || !data->ast || !data->ast->value)
		return ;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	apply_redirections_for_builtin(data);
	builtin_result = is_builtins(data, data->ast->value);
	(void)builtin_result;
	restore_redirections_for_builtin(data, saved_stdin, saved_stdout);
}
