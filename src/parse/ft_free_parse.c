/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:24:13 by paromero          #+#    #+#             */
/*   Updated: 2025/01/24 11:33:37 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
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

// Añade estas funciones en minishell.c o donde sea apropiado
void	ft_free_ast(t_ast *node)
{
    if (!node)
        return;
    ft_free_ast(node->left);
    ft_free_ast(node->right);
    free(node->value);
    if (node->args)
    {
        int i = 0;
        while (node->args[i])
            free(node->args[i++]);
        free(node->args);
    }
    free(node);
}

void	ft_free_env(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env;
        env = env->next;
        free(tmp->value);
        free(tmp);
    }
}

void	cleanup_resources(t_data *data)
{
    if (data->line)
    {
        free(data->line); // Liberar línea de input
        data->line = NULL;
    }
    if (data->tokens)
    {
        ft_free_tokens(data->tokens); // Liberar tokens
        data->tokens = NULL;
    }
    if (data->ast)
    {
        ft_free_ast(data->ast); // Liberar AST
        data->ast = NULL;
    }
    if (data->env)
    {
        ft_free_env(data->env); // Liberar lista de entorno
        data->env = NULL;
    }
    if (data->cwd)
    {
        free(data->cwd); // Liberar directorio actual
        data->cwd = NULL;
    }
}

void	ft_free_error_token(t_data	*data, char **result)
{
	ft_free_split(result);
	ft_free_tokens(data->tokens);
}