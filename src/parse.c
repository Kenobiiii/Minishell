/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 20:08:34 by paromero          #+#    #+#             */
/*   Updated: 2024/11/28 17:56:38 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * DONE arreglar segfault llega hasta 3 palabras y luego segfault
 * DONE función que te dice el tipo
 * DONE func separador
 * DONE func de tokens
 * TODO arreglar core dumped con este ejemplo "                                                                           ls | cmd <<asd<><<asd && | | & &"
 * TODO func de ast
 */

int	ft_types(char	*value)
{
	if (value[0] == '|' && value[1] != '|')
		return (PIPE);
	if (value[0] == '>' && value[1] == '>')
		return (REDOUT2);
	if (value[0] == '>')
		return (REDIRECT_OUT);
	if (value[0] == '<' && value[1] == '<')
		return (REDIN2);
	if (value[0] == '<')
		return (REDIRECT_IN);
	if (value[0] == '&' && value[1] == '&')
		return (AND);
	if (value[0] == '|' && value[1] == '|')
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
			if (ft_types(line + i) == REDOUT2 || ft_types(line + i) == REDIN2
			 || ft_types(line + i) == AND || ft_types(line + i) == OR)
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

t_tokens	*ft_new_token(char	*str)
{
	t_tokens	*new_node;

	new_node = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(str);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = (NULL);
	return (new_node);
}

int	ft_tokens(t_data *data, char *str)
{
	t_tokens	*current;
	char	**result;
	int		i;

	result = ft_split(str, ' ');
	data->tokens = ft_new_token(result[0]);
	if (!data->tokens)
		return (0);
	current = data->tokens;
	i = 1;
	while (result[i])
	{
		current->type = ft_types(result[i - 1]);
		current->next = ft_new_token(result[i]);
		if (!current->next)
		{
			ft_free_split(result);
			ft_free_tokens(data->tokens);
			return (0);
		}
		current = current->next;
		i++;
	}
	current->type = ft_types(result[i - 1]);
	ft_free_split(result);
	return (1);
}

