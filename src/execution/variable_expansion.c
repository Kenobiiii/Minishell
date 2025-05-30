/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/05/30 16:36:20 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Expands variables in command arguments at execution time
 * This allows variables to be properly expanded after export commands
 * in logical operators like &&
 */
void	expand_command_variables(t_data *data, t_ast *node)
{
	int		i;
	char	*expanded;

	if (!node || node->type != CMD || !node->args)
		return ;
	
	i = 0;
	while (node->args[i])
	{
		// Check if the argument contains variables that need expansion
		if (ft_strchr(node->args[i], '$'))
		{
			expanded = ft_handledollar(data, node->args[i]);
			if (expanded)
			{
				free(node->args[i]);
				node->args[i] = expanded;
			}
		}
		i++;
	}
}
