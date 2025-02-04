/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:47:59 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 12:39:33 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_dobletype(t_type	type)
{
	if (type == REDOUT2 || type == REDIN2
		|| type == AND || type == OR)
		return (1);
	return (0);
}

size_t	ft_spacestrlen(char *line)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (line[i] != '\0')
	{
		j++;
		if (ft_types(line + i) != CMD)
		{
			j += 2;
			if (line[i + 1] && line[i + 1] == line[i])
				i++;
		}
		i++;
	}
	return (j);
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

