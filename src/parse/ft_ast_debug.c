/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:30:00 by paromero          #+#    #+#             */
/*   Updated: 2025/05/21 17:30:00 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_type_str(t_type type)
{
	if (type == CMD)
		return ("CMD");
	if (type == REDIRECT_OUT)
		return ("REDIRECT_OUT");
	if (type == REDIRECT_IN)
		return ("REDIRECT_IN");
	if (type == REDIN2)
		return ("REDIN2");
	if (type == REDOUT2)
		return ("REDOUT2");
	if (type == PIPE)
		return ("PIPE");
	if (type == AND)
		return ("AND");
	if (type == OR)
		return ("OR");
	return ("UNKNOWN");
}

static void	print_node(t_ast *node, int level)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i < level)
	{
		printf("  ");
		i++;
	}
	printf("└─ Type: %s\n", get_type_str(node->type));
	i = 0;
	while (i < level + 1)
	{
		printf("  ");
		i++;
	}
	printf("Value: %s\n", node->value ? node->value : "NULL");
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			int j = 0;
			while (j < level + 1)
			{
				printf("  ");
				j++;
			}
			printf("Arg[%d]: %s\n", i, node->args[i]);
			i++;
		}
	}
	printf("\n");
	print_node(node->left, level + 1);
	print_node(node->right, level + 1);
}

void	print_ast(t_ast *root)
{
	printf("\n=== AST Structure ===\n");
	if (!root)
	{
		printf("Empty AST\n");
		printf("===================\n\n");
		return ;
	}
	print_node(root, 0);
	printf("===================\n\n");
} 