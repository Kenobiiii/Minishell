/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 20:08:34 by paromero          #+#    #+#             */
/*   Updated: 2025/01/15 17:51:08 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * DONE arreglar segfault llega hasta 3 palabras y luego segfault
 * DONE función que te dice el tipo
 * DONE func separador
 * DONE func de tokens
 * DONE func de ast
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

int	ft_tokens(t_data *data, char *str)
{
	t_tokens	*current;
	char		**result;
	int			i;

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
			ft_free_error_token(data, result);
			return (0);
		}
		current = current->next;
		i++;
	}
	current->type = ft_types(result[i - 1]);
	ft_free_split(result);
	return (1);
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
