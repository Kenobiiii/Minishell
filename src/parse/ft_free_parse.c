/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:24:13 by paromero          #+#    #+#             */
/*   Updated: 2025/01/29 18:19:47 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_matrix(char **array)
{
	size_t	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_free_tokens(t_tokens *tokens)
{
	t_tokens	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

void	ft_free_ast(t_ast	*ast)
{
	t_ast	*temp;

	while (ast)
	{
		temp = ast;
		ast = ast->right;
		free(temp->value);
		if (ast->args)
			free_matrix(ast->args);
		free(temp);
	}
}

void	ft_free_error_token(t_data	*data, char **result)
{
	free_matrix(result);
	ft_free_tokens(data->tokens);
}
