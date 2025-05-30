/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/05/30 12:13:43 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_shell_state = STATE_PROMPT_NORMAL;

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
		if (!ft_isspace(data->line))
		{
			free_while(data);
			return ;
		}
		if (!line_syntax(data))
		{
			free_while(data);
			return ;
		}
		g_shell_state = STATE_EXECUTING;
		setup_signals_for_execution();
		if (is_builtin_command(data->ast->value))
			execute_builtin_with_redirections(data);
		else
			exec_func(data);
	}
}

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
	return (data.last_exit_status);
}

int	main(int ac, char **av, char **env)
{
	int	exit_status;

	if (ac != 1)
	{
		printf("Error: More than one arguments\n");
		return (0);
	}
	if (env == NULL || env[0] == NULL)
	{
		printf("Error: No enviroment found\n");
		return (0);
	}
	(void)ac;
	(void)av;
	exit_status = minishell(env);
	return (exit_status);
}
