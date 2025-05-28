/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:00:00 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/27 19:26:06 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_line_errors(t_data *data)
{
	if (openquotes(data->line))
	{
		data->wstatus = 2;
		printf("minishell: syntax error: unclosed quotes\n");
		return (0);
	}
	if (!check_syntax(data->line))
	{
		data->wstatus = 2;
		return (0);
	}
	return (1);
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
