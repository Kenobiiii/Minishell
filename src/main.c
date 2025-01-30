/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2025/01/30 11:37:43 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t g_sigint_received = 0;

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

int	update_pwd(t_data	*data)
{
	char	*new_pwd;
	char	cwd[PATH_MAX];
	t_env	*tmp;

	new_pwd = getcwd(cwd, sizeof(cwd));
	tmp = data->env;
	while (ft_strncmp(tmp->value, "PWD=", 4) != 0)
		tmp = tmp->next;
	if (tmp)
	{
		free (tmp->value);
		tmp->value = ft_strcat("PWD=", new_pwd);
	}
	free (data->cwd);
	data->cwd = ft_strdup(new_pwd);
	
	return (1);
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
	setup_signals(); // Configurar señales al inicio
	while (data.exit == 0)
	{
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			free(data.line);
			data.line = NULL;
		}
		update_pwd(&data);
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
			if (ft_strncmp(data.line, "pwd", 3) == 0)
				printf("%s\n", data.cwd);
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
