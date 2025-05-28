/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/05/28 15:55:23 by paromero         ###   ########.fr       */
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
	if (!handle_invslash_pcomma(data->line))
	{
		data->wstatus = 2;
		return (0);
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
