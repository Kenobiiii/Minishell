/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:30 by paromero          #+#    #+#             */
/*   Updated: 2024/12/17 18:41:06 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*ft_new_node(t_type type, char *value)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->args = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_ast	*ft_ast(t_tokens *tokens)
{
	t_ast		*root;
	t_ast		*current;
	t_ast		*new_node;
	t_tokens	*tmp;
	int			i;
	int			arg_count;

	root = NULL;
	current = NULL;
	while (tokens)
	{
		if (tokens->type == CMD)
		{
			new_node = ft_new_node(CMD, tokens->value);
			tmp = tokens->next;
			arg_count = 0;
			while (tmp && tmp->type == CMD)
			{
				arg_count++;
				tmp = tmp->next;
			}
			if (arg_count > 0)
			{
				new_node->args = malloc(sizeof(char *) * (arg_count + 2));
				new_node->args[0] = ft_strdup(tokens->value);
				i = 1;
				tokens = tokens->next;
				while (tokens && tokens->type == CMD)
				{
					new_node->args[i++] = ft_strdup(tokens->value);
					tokens = tokens->next;
				}
				new_node->args[i] = NULL;
			}
			else
				tokens = tokens->next;
			if (!root)
				root = new_node;
			else if (current && current->type == PIPE)
				current->right = new_node;
			else
				current->left = new_node;
			current = new_node;
		}
		else if (tokens->type == PIPE)
		{
			new_node = ft_new_node(PIPE, tokens->value);
			new_node->left = root;
			root = new_node;
			current = new_node;
			tokens = tokens->next;
		}
		else if (tokens->type == REDIRECT_OUT || tokens->type == REDIRECT_IN
			|| tokens->type == REDIN2 || tokens->type == REDOUT2)
		{
			new_node = ft_new_node(tokens->type, tokens->value);
			tokens = tokens->next;
			if (tokens && tokens->type == CMD)
			{
				new_node->right = ft_new_node(CMD, tokens->value);
				tokens = tokens->next;
			}
			if (current)
				current->right = new_node;
		}
		else
			tokens = tokens->next;
	}
	return (root);
}
