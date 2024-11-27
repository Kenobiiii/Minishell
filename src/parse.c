/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 20:08:34 by paromero          #+#    #+#             */
/*   Updated: 2024/11/27 19:13:33 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * DONE arreglar segfault llega hasta 3 palabras y luego segfault
 * DONE función que te dice el tipo
 * TODO func separador
 * TODO func de tokens
 * TODO func de ast
 */

int	ft_types(char	*value)
{
	if (ft_strncmp(value, "|", 2) == 0)
		return (PIPE);
	if (ft_strncmp(value, ">", 2) == 0 || ft_strncmp(value, ">>", 3) == 0
		|| ft_strncmp(value, "<", 2) == 0 || ft_strncmp(value, "<<", 3) == 0)
		return (REDIRECT);
	if (ft_strncmp(value, "&&", 2) == 0)
		return (AND);
	if (ft_strncmp(value, "||", 2) == 0)
		return (OR);
	return (CMD);
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

char	*ft_spaces(char *line)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = (char *)malloc(sizeof(char) * (ft_spacestrlen(line) + 1));
	if (!result)
		return (NULL);
	while (line[i] != '\0')
	{
		if (ft_types(line + i) != CMD)
		{
			result[j++] = ' ';
			result[j++] = line[i];
			if (ft_types(line[i + 1]) == REDIRECT)
				result[j++] = line[++i];
			result[j++] = ' ';
		}
		else
		{
			result[j++] = line[i];
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}


void	ft_tokens(char *str)
{
	char	**result;
	int		i;

	i = 0;
	result = ft_split(str, ' ');
	if (ft_types(str) == REDIRECT)
		printf("pipe\n");
	while (result[i] != NULL)
	{
		printf("palabra %d: %s\n", i, result[i]);
		i++;
	}
}
