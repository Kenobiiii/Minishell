/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:30 by paromero          #+#    #+#             */
/*   Updated: 2025/04/07 13:13:49 by paromero         ###   ########.fr       */
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
	if (cmd_node->args == NULL)
	{
		new_args[0] = ft_strdup(cmd_node->value);
		j = 1;
	}
	else
		while (j < i)
		{
			new_args[j] = ft_strdup(cmd_node->args[j]);
			j++;
		}
	new_args[j] = ft_strdup(arg);
	new_args[j + 1] = NULL;
	free_matrix(cmd_node->args);
	cmd_node->args = new_args;
}

int	is_red(t_ast **last_op)
{
	if (*last_op && ((*last_op)->type == REDIN2 || (*last_op)->type == REDOUT2
			|| (*last_op)->type == REDIRECT_IN
			|| (*last_op)->type == REDIRECT_OUT))
		return (1);
	return (0);
}

static void	handle_command_as_arg(t_ast **cmd, t_ast **redirect,
		t_tokens *tokens)
{
	if (*cmd && (*cmd)->type == CMD)
		ft_add_argument(*cmd, tokens->value);
	else
		(*redirect)->right = ft_create_ast_node(CMD, tokens->value);
}

static void	handle_redirection(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*original_cmd;

	original_cmd = *cmd;
	if (is_redin2(last_op))
		redin2(cmd, last_op, NULL, tokens);
	else
		(*last_op)->right = ft_create_ast_node(CMD, tokens->value);
	if (!(*last_op)->left && original_cmd)
		(*last_op)->left = original_cmd;
	if (!*root)
		*root = *last_op;
	*cmd = *last_op;
	*last_op = NULL;
}

static void	handle_new_command(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*new_cmd;

	new_cmd = ft_create_ast_node(CMD, tokens->value);
	if (!new_cmd)
		return ;
	new_cmd->args = malloc(sizeof(char *) * 2);
	if (new_cmd->args)
	{
		new_cmd->args[0] = ft_strdup(tokens->value);
		new_cmd->args[1] = NULL;
	}
	*cmd = new_cmd;
	if (!*root)
		*root = *cmd;
	else if (*last_op)
		(*last_op)->right = *cmd;
}

void	ft_handle_command_node(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*cmd_redir;

	if (is_red(last_op))
	{
		cmd_redir = (*cmd && (*cmd)->type == CMD) ? *cmd : NULL;
		handle_redirection(root, cmd, last_op, tokens);
		if (cmd_redir)
			(*cmd)->left = cmd_redir;
	}
	else if (*cmd)
	{
		if ((*cmd)->type == CMD)
			ft_add_argument(*cmd, tokens->value);
		else if ((*cmd)->left && (*cmd)->left->type == CMD)
			ft_add_argument((*cmd)->left, tokens->value);
		else
			handle_command_as_arg(cmd, cmd, tokens);
	}
	else
		handle_new_command(root, cmd, last_op, tokens);
}

static void	connect_operator(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_ast *new_op)
{
	new_op->left = *cmd;
	if (*last_op)
		(*last_op)->right = new_op;
	else
		*root = new_op;
	*last_op = new_op;
	if (!is_red(last_op))
		*cmd = NULL;
}

void	ft_handle_operator_node(t_ast **root, t_ast **cmd,
		t_ast **last_op, t_tokens *tokens)
{
	t_ast	*new_op;

	new_op = ft_create_ast_node(tokens->type, tokens->value);
	if (!new_op)
		return ;
	if (*cmd)
		connect_operator(root, cmd, last_op, new_op);
	else
	{
		*root = new_op;
		*last_op = new_op;
	}
}

t_ast	*ft_build_ast(t_tokens *tokens)
{
	t_ast	*root;
	t_ast	*cmd;
	t_ast	*last_op;

	root = NULL;
	cmd = NULL;
	last_op = NULL;
	while (tokens)
	{
		if (tokens->type == CMD)
			ft_handle_command_node(&root, &cmd, &last_op, tokens);
		else
			ft_handle_operator_node(&root, &cmd, &last_op, tokens);
		tokens = tokens->next;
	}
	return (root);
}