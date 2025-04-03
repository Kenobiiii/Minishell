/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:30 by paromero          #+#    #+#             */
/*   Updated: 2025/04/01 11:42:56 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	is_red(t_ast **last_op)
{
	if (*last_op)
	{
		if ((*last_op)->type == REDIN2 || (*last_op)->type == REDOUT2
			|| (*last_op)->type == REDIRECT_IN
			|| (*last_op)->type == REDIRECT_OUT)
			return (1);
	}
	return (0);
}

//! Nuevo cambio
void	ft_handle_command_node(t_ast **root, t_ast **current_cmd,
	t_ast **last_operator, t_tokens *tokens)
{
	t_ast	*new_node;

	if (!*current_cmd && tokens->type == CMD)
		new_node = ft_create_ast_node(CMD, tokens->value);
	if (is_red(last_operator))
	{
		if (is_redin2(last_operator))
			redin2(current_cmd, last_operator, new_node, tokens);
		else
			(*last_operator)->right = ft_create_ast_node(CMD, tokens->value);
		*current_cmd = (*last_operator)->left;
		*last_operator = NULL;
	}
	else
	{
		if (*current_cmd)
			ft_add_argument(*current_cmd, tokens->value);
		else
		{
			new_node->args = malloc(sizeof(char *) * 2);
			if (new_node->args)
			{
				new_node->args[0] = ft_strdup(tokens->value);
				new_node->args[1] = NULL;
			}
			*current_cmd = new_node;
			if (!*root)
				*root = *current_cmd;
			else if (*last_operator)
				(*last_operator)->right = *current_cmd;
		}
	}
}

void	ft_handle_operator_node(t_ast **root, t_ast **current_cmd,
	t_ast **last_operator, t_tokens *tokens)
{
	t_ast	*new_node;

	new_node = ft_create_ast_node(tokens->type, tokens->value);
	if (*current_cmd)
	{
		new_node->left = *current_cmd;
		if (*last_operator)
			(*last_operator)->right = new_node;
		else
			*root = new_node;
		*last_operator = new_node;
		*current_cmd = NULL;
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
	while (cmd_node->args && cmd_node->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	new_args[0] = ft_strdup(cmd_node->value);
	j = 1;
	while (j < i)
	{
		new_args[j] = ft_strdup(cmd_node->args[j]);
		j++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free_matrix(cmd_node->args);
	cmd_node->args = new_args;
}
