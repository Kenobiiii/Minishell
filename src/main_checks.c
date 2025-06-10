/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:30:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 15:24:37 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_line_errors(t_data *data)
{
	if (!handle_invslash_pcomma(data->line))
	{
		data->wstatus = 2;
		return (0);
	}
	if (openquotes(data->line))
	{
		data->wstatus = 2;
		printf("minishell: syntax error: unclosed quotes\n");
		return (0);
	}
	if (!check_syntax(data->line))
	{
		data->wstatus = 2;
		syntax_error();
		return (0);
	}
	return (1);
}

int	is_only_empty_var(char *line)
{
	if (!line)
		return (0);
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

	if (!line || !data)
		return (0);
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
