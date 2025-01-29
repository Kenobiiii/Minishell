/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:24:13 by paromero          #+#    #+#             */
/*   Updated: 2025/01/29 21:03:24 by anggalle         ###   ########.fr       */
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
	if (!ast)
		return;
	ft_free_ast(ast->left);
	ft_free_ast(ast->right);
	free (ast->value);
	if (ast->args)
		free_matrix(ast->args);
	free (ast);
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
void	ft_free_error_token(t_data	*data, char **result)
{
	free_matrix(result);
	ft_free_tokens(data->tokens);
}
