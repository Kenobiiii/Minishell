/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 20:08:34 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 13:17:29 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	ft_tokens(t_data *data, char *str)
{
	t_tokens	*current;
	char		**result;
	int			i;

	result = ft_quotesplit(str, ' ', data);
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
	free_matrix(result);
	return (1);
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
