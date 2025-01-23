/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:47:59 by paromero          #+#    #+#             */
/*   Updated: 2025/01/23 12:47:38 by paromero         ###   ########.fr       */
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

int	ft_syntax(t_data *data)
{
	t_tokens	*current;
	t_tokens	*previous;

	current = data->tokens;
	if (current->type != CMD)
	{
		printf("Syntax error\n");
		return (0);
	}
	while (current)
	{
		if (previous && previous->type != CMD && current->type != CMD)
		{
			printf("Syntax error\n");
			return (0);
		}
		previous = current;
		current = current->next;
	}
	if (previous && previous->type != CMD)
	{
		printf("Syntax error\n");
		return (0);
	}
	return (1);
}

char	*ft_delete_spaces(char *line)
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
			if (ft_dobletype(ft_types(line + i)))
				result[j++] = line[++i];
			result[j++] = ' ';
		}
		else
			result[j++] = line[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
