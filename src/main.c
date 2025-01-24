/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/01/24 11:27:16 by paromero         ###   ########.fr       */
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

char	*deletefirstspaces(char	*line)
{
	char	*new_line;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	while (line[i] == ' ')
		i++;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) - i + 1));
	if (!new_line)
	{
		free (line);
		return (NULL);
	}
	while (line[i] != '\0')
	{
		new_line[j] = line[i];
		i++;
		j++;
	}
	free (line);
	new_line[j] = '\0';
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
			continue;
		}
	}
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
