/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/05/02 14:37:46 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_sigint_received = 0;

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
	if (!ft_isspace(data->line))
		return (free_while(data));
	if (!check_syntax(data->line))
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		data->wstatus = 2;
		return (free_while(data));
	}
	if (!openquotes(data->line))
	{
		perror("command not found");
		data->wstatus = 127;
		return (free_while(data));
	}
	if (!handle_invslash_pcomma(data->line))
		return (free_while(0));
	ft_tokens(data, data->line);
	data->ast = ft_build_ast(data, data->tokens);
	if (data->ast == NULL)
		return (free_while(data));
	return (1);
}

int	minishell(char **env)
{
	t_data	data;

	init_data(&data, env);
	setup_signals();
	while (data.exit == 0)
	{
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			data.wstatus = 130;
			if (data.line)
			{
				free(data.line);
				data.line = NULL;
			}
		}
		update_pwd(&data);
		data.line = readline(data.prompt);
		if (data.line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (!line_syntax(&data))
			continue ;
		if (is_builtins(&data, data.ast->value) == 0)
			exec_func(&data);
		free_while(&data);
		continue ;
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
