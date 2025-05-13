/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redin2_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 13:19:35 by paromero          #+#    #+#             */
/*   Updated: 2025/02/06 13:56:36 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//? Verifica si el último operador es un operador de heredoc (<<)
int	is_redin2(t_ast **last_operator)
{
	if (*last_operator)
	{
		if ((*last_operator)->type == REDIN2)
			return (1);
	}
	return (0);
}

//? Maneja el caso especial de heredoc (<<) con diferentes configuraciones de nodos
void	redin2(t_ast **current_cmd, t_ast **last_operator,
		t_ast *new_node, t_tokens *tokens)
{
	if (!(*last_operator)->left)
		(*last_operator)->left = new_node;
	else
	{
		if (*current_cmd)
		{
			ft_add_argument(*current_cmd, tokens->value);
			free(new_node->value);
			free(new_node);
		}
		else
		{
			new_node->args = malloc(sizeof(char *) * 2);
			if (new_node->args)
			{
				new_node->args[0] = tokens->value;
				new_node->args[1] = NULL;
			}
			*current_cmd = new_node;
			(*last_operator)->right = new_node;
		}
	}
}
