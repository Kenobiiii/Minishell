/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:00:00 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/27 19:26:06 by paromero         ###   ########.fr       */
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

int	line_syntax(t_data	*data)
{
	add_history(data->line);
	if (is_only_empty_var(data->line))
	{
		data->wstatus = 0;
		return (free_while(data));
	}
	if (is_empty_var_with_cmd(data, data->line))
	{
	}
	if (!check_line_errors(data))
		return (0);
	ft_tokens(data, data->line);
	data->ast = ft_build_ast(data, data->tokens);
	if (data->ast == NULL)
		return (free_while(data));
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

int	is_only_empty_var(char *line)
{
	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	if (ft_strncmp(line, "$EMPTY", 6) == 0)
	{
		line += 6;
		while (*line)
		{
			if (*line != ' ' && *line != '\t')
				return (0);
			line++;
		}
		return (1);
	}
	return (0);
}

int	is_empty_var_with_cmd(t_data *data, char *line)
{
	char	*temp;

	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	if (ft_strncmp(line, "$EMPTY", 6) == 0 && line[6] == ' ')
	{
		temp = ft_strdup(line + 7);
		if (!temp)
			return (0);
		free(data->line);
		data->line = temp;
		return (1);
	}
	return (0);
}
