/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/01/29 20:23:45 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isSpace(char	*line)
{
	int	i;

	i = 0;
	if (ft_strlen(line) == 0)
		return (0);
	while (line[i])
	{
		if (line[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

char *deletefirstspaces(char *line) {
	char	*new_line;
	int		i;

	i = 0;
	while (line[i] == ' ')
		i++;
	new_line = ft_strdup(line + i);
	free(line);
	return (new_line);
}

int	minishell(char **env)
{
	t_data	data;

	init_data(&data, env);
	while (data.exit == 0)
	{
		data.line = readline(data.prompt);
		if (data.line == NULL)
			break ;
		if (!ft_isSpace(data.line))
			continue ;
		add_history(data.line);
		if (ft_strncmp(data.line, "exit", 5) == 0) //TODO quitarlo
			data.exit = 1;
		data.line = deletefirstspaces(data.line);
		if (!openquotes(data.line))
		{
			printf("Syntax error\n");
			continue ;
		}
		data.line = ft_delete_spaces(data.line);
		ft_tokens(&data, data.line);
		if (data.line && ft_syntax(&data))
		{
			//print_tokens(data.tokens);
			data.ast = ft_build_ast(data.tokens);
			//print_ast(data.ast, 1);
			if (is_builtins(&data) == 0)
			{
				exec_func(&data);
			}
			free_innerwhile(&data);
			continue;
		}
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
