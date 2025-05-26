/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/05/26 10:19:19 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Definición de la ÚNICA variable global para el estado de las señales
volatile sig_atomic_t	g_shell_state = STATE_PROMPT_NORMAL;

static	int	is_only_empty_var(char *line)
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

static	int	is_empty_var_with_cmd(t_data *data, char *line)
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
