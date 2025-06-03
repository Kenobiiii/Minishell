/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_multi.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/03 10:30:07 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

static int	handle_single_redirection(t_data *data, t_ast *node)
{
	if (node->type == REDIRECT_OUT)
		exec_redirect_out(data, node);
	else if (node->type == REDOUT2)
		exec_redirect_append(data, node);
	return (1);
}

static t_ast	**allocate_redirections_array(int count)
{
	t_ast	**redirections;

	redirections = malloc(sizeof(t_ast *) * count);
	return (redirections);
}

static void	process_redirections_array(t_data *data, t_ast *node, int count)
{
	t_ast	**redirections;
	int		index;

	redirections = allocate_redirections_array(count);
	if (!redirections)
		return ;
	index = 0;
	collect_redirections(node, redirections, &index);
	if (process_redirection_files(data, redirections, count) == -1)
	{
		free(redirections);
		return ;
	}
	free(redirections);
	execute_command_after_redirections(data, node);
}

void	process_multiple_output_redirections(t_data *data, t_ast *node)
{
	int	count;

	count = count_output_redirections(node);
	if (count <= 1)
	{
		handle_single_redirection(data, node);
		return ;
	}
	process_redirections_array(data, node, count);
}
