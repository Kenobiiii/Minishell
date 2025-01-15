/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:30 by paromero          #+#    #+#             */
/*   Updated: 2025/01/15 17:49:57 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*ft_create_ast_node(t_type type, char *value)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	node->args = NULL;
	node->right = NULL;
	node->left = NULL;
	return (node);
}

void	ft_handle_command_node(t_ast **root, t_ast **current_cmd,
	t_ast **last_operator, t_tokens *tokens)
{
	t_ast	*new_node;

	new_node = ft_create_ast_node(CMD, tokens->value);
	if (*current_cmd)
		ft_add_argument(*current_cmd, tokens->value);
	else
	{
		*current_cmd = new_node;
		if (!*root)
			*root = *current_cmd;
		else if (*last_operator)
			(*last_operator)->right = *current_cmd;
	}
}

void	ft_handle_operator_node(t_ast **root, t_ast **current_cmd,
	t_ast **last_operator, t_tokens *tokens)
{
	t_ast	*new_node;

	new_node = ft_create_ast_node(tokens->type, tokens->value);
	if (*root)
	{
		new_node->left = *root;
		*root = new_node;
		*last_operator = new_node;
		if (*current_cmd)
		{
			(*last_operator)->right = *current_cmd;
			*current_cmd = NULL;
		}
	}
	else
	{
		*root = new_node;
		*last_operator = new_node;
	}
}

t_ast	*ft_build_ast(t_tokens *tokens)
{
	t_ast	*root;
	t_ast	*current_cmd;
	t_ast	*last_operator;

	root = NULL;
	current_cmd = NULL;
	last_operator = NULL;
	while (tokens)
	{
		if (tokens->type == CMD)
			ft_handle_command_node(&root, &current_cmd, &last_operator, tokens);
		else
			ft_handle_operator_node(&root, &current_cmd,
				&last_operator, tokens);
		tokens = tokens->next;
	}
	return (root);
}

void	ft_add_argument(t_ast *cmd_node, char *arg)
{
	int		i;
	int		j;
	char	**new_args;

	i = 0;
	j = 0;
	while (cmd_node->args && cmd_node->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	while (j < i)
	{
		new_args[j] = cmd_node->args[j];
		j++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd_node->args);
	cmd_node->args = new_args;
}
