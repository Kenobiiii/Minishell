/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:52:10 by paromero          #+#    #+#             */
/*   Updated: 2025/06/02 17:33:46 by paromero         ###   ########.fr       */
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

int	openquotes(char	*line)
{
	int	i;
	int	count_double;
	int	count_single;

	count_single = 0;
	count_double = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '"' && count_single % 2 == 0)
			count_double++;
		if (line[i] == '\'' && count_double % 2 == 0)
			count_single++;
		i++;
	}
	if (count_double % 2 != 0 || count_single % 2 != 0)
		return (1);
	return (0);
}

int	handle_invslash_pcomma(char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\\' || line[i] == ';')
			return (syntax_error());
		i++;
	}
	return (1);
}
