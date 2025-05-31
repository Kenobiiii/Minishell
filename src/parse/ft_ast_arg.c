/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:29:00 by paromero          #+#    #+#             */
/*   Updated: 2025/05/28 17:27:02 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Copies existing arguments to a new array and adds a new argument
void	copy_args(char **new_args, t_ast *cmd_node, int i, char *arg)
{
	int	j;

	j = 0;
	if (cmd_node->args == NULL)
	{
		new_args[0] = ft_strdup(cmd_node->value);
		new_args[1] = ft_strdup(arg);
		new_args[2] = NULL;
		return ;
	}
	while (j < i)
	{
		new_args[j] = ft_strdup(cmd_node->args[j]);
		j++;
	}
	new_args[j] = ft_strdup(arg);
	new_args[j + 1] = NULL;
}

//? Adds a new argument to an existing command node
void	ft_add_argument(t_ast *cmd_node, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	while (cmd_node->args && cmd_node->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	copy_args(new_args, cmd_node, i, arg);
	free_matrix(cmd_node->args);
	cmd_node->args = new_args;
}

//? Handles a command argument in the context of a redirection
void	handle_command_as_arg(t_ast **cmd, t_ast **redirect,
	t_tokens *tokens)
{
	t_ast	*new_node;

	if (*cmd && (*cmd)->type == CMD)
		ft_add_argument(*cmd, tokens->value);
	else
	{
		new_node = ft_create_ast_node(CMD, tokens->value);
		if (new_node)
			(*redirect)->right = new_node;
	}
}
