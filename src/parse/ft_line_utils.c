/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:52:10 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 17:53:40 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(char	*line)
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

char	*deletefirstspaces(char *line)
{
	char	*new_line;
	int		i;

	i = 0;
	while (line[i] == ' ')
		i++;
	new_line = ft_strdup(line + i);
	free(line);
	return (new_line);
}

int	openquotes(char	*line)
{
	int	i;
	int	count_double;
	int	count_single;

	count_single = 0;
	count_double = 0;
	i = 0;
	if (line[i] == '"' || line[0] == '\'')
		return (0);
	while (line[i])
	{
		if (line[i] == '"' && count_single % 2 == 0)
			count_double++;
		if (line[i] == '\'' && count_double % 2 == 0)
			count_single++;
		i++;
	}
	if (count_double % 2 == 0 && count_single % 2 == 0)
		return (1);
	return (0);
}

int	handle_invslash_pcomma(char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
			quote = line[i];
		while (line[i] != quote && quote)
			i++;
		if (line[i] == '\\' || line[i] == ';')
			return (syntax_error());
		i++;
	}
	return (1);
}
