/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:24:13 by paromero          #+#    #+#             */
/*   Updated: 2025/02/20 12:26:11 by anggalle         ###   ########.fr       */
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
	if (array)
	{
		free(array);
		array = NULL;
	}
}

void	ft_free_tokens(t_tokens *tokens)
{
	t_tokens	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->value)
			free(temp->value);
		if (temp)
			free(temp);
		temp = NULL;
	}
}

void	ft_free_ast(t_ast	*ast)
{
	if (!ast)
		return ;
	
	if (ast->left)
		ft_free_ast(ast->left);
	if (ast->right)
		ft_free_ast(ast->right);
	if (ast->value)
		free (ast->value);
	if (ast->args)
		free_matrix(ast->args);
	free (ast);
	ast = NULL;
}

void	ft_free_env(t_env	*env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->value);
		free(temp);
	}
}

void	ft_free_error_token(t_data	*data, char **array)
{
	free_matrix(array);
	ft_free_tokens(data->tokens);
}
