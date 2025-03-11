/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:52:10 by paromero          #+#    #+#             */
/*   Updated: 2025/03/11 18:08:54 by paromero         ###   ########.fr       */
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
		{
			quote = line[i];
			i++;
			while (line[i] != quote)
				i++;

			quote = 0;
		}
		else if (line[i] == '\\' || line[i] == ';')
		{
			return (syntax_error());
		}
		i++;
	}
	return (1);
}

char	*ft_mask_operator(char *str)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = malloc(ft_strlen(str) * 2 + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '&' && str[i + 1] == '&') || (str[i] == '|'
				&& str[i + 1] == '|'))
			result[j++] = '\x01';
		else if (str[i] == '|' || str[i] == '>' || str[i] == '<')
			result[j++] = '\x01';
		result[j++] = str[i++];
	}
	result[j] = '\0';
	free(str);
	return (result);
}
