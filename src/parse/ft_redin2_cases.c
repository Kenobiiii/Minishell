/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redin2_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 13:19:35 by paromero          #+#    #+#             */
/*   Updated: 2025/05/25 17:33:18 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Verifica si el último operador es un operador de heredoc (<<)
int	is_redin2(t_ast **last_operator)
{
	if (last_operator && *last_operator)
	{
		if ((*last_operator)->type == REDIN2)
			return (1);
	}
	return (0);
}

//? Maneja argumentos adicionales cuando ya hay comando y delimitador
static void	handle_additional_args(t_ast **current_cmd, t_ast *new_node,
		t_tokens *tokens)
{
	if (current_cmd && *current_cmd)
	{
		ft_add_argument(*current_cmd, tokens->value);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
	}
	else
	{
		new_node->args = malloc(sizeof(char *) * 2);
		if (new_node->args)
		{
			new_node->args[0] = ft_strdup(tokens->value);
			new_node->args[1] = NULL;
		}
		if (current_cmd)
			*current_cmd = new_node;
	}
}

//? Maneja el caso especial de heredoc (<<) con configuraciones de nodos
void	redin2(t_ast **current_cmd, t_ast **last_operator,
		t_ast *new_node, t_tokens *tokens)
{
	if (!last_operator || !*last_operator || !new_node || !tokens)
		return ;
	if (!(*last_operator)->left)
		(*last_operator)->left = new_node;
	else if (!(*last_operator)->right)
		(*last_operator)->right = new_node;
	else
		handle_additional_args(current_cmd, new_node, tokens);
}
