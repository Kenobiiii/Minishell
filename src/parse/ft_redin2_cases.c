/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redin2_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 13:19:35 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 13:19:55 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redin2(t_ast **last_operator)
{
	if (*last_operator)
	{
		if ((*last_operator)->type == REDIN2)
			return (1);
	}
	return (0);
}

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
		}
		else
		{
			new_node->args = malloc(sizeof(char *) * 2);
			if (new_node->args)
			{
				new_node->args[0] = ft_strdup(tokens->value);
				new_node->args[1] = NULL;
			}
			*current_cmd = new_node;
			(*last_operator)->right = new_node;
		}
	}
}