/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:45:00 by paromero          #+#    #+#             */
/*   Updated: 2025/05/28 16:44:29 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

int	write_heredoc_line(int pipefd, char *line)
{
	if (write(pipefd, line, ft_strlen(line)) == -1
		|| write(pipefd, "\n", 1) == -1)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (1);
}

t_ast	*find_cmd_node(t_ast *node)
{
	t_ast	*current;

	if (!node)
		return (NULL);
	current = node;
	while (current && (current->type == REDIRECT_OUT
			|| current->type == REDOUT2))
		current = current->left;
	return (current);
}

int	count_output_redirections(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == REDIRECT_OUT || node->type == REDOUT2)
		return (1 + count_output_redirections(node->left));
	return (0);
}

void	collect_redirections(t_ast *node, t_ast **redirections, int *index)
{
	if (!node)
		return ;
	if (node->type == REDIRECT_OUT || node->type == REDOUT2)
	{
		collect_redirections(node->left, redirections, index);
		redirections[(*index)++] = node;
	}
}

int	open_redirection_file(t_ast *redirection)
{
	int	fd;

	if (redirection->type == REDIRECT_OUT)
		fd = open(redirection->right->value, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	else
		fd = open(redirection->right->value, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	return (fd);
}
