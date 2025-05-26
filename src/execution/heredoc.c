/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:14:52 by paromero          #+#    #+#             */
/*   Updated: 2025/05/26 22:14:52 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

t_ast	*find_last_redirection(t_ast *node)
{
	if (!node || (node->type != REDIRECT_OUT && node->type != REDOUT2))
		return (NULL);
	return (node);
}

static int	validate_heredoc_node(t_data *data, t_ast *node, int original_stdin)
{
	if (!node)
	{
		ft_putstr_fd("error: heredoc node is NULL\n", STDERR_FILENO);
		data->wstatus = 1;
		return (-1);
	}
	if (!node->right || !node->right->value)
	{
		ft_putstr_fd("syntax error: missing delimiter for <<\n", STDERR_FILENO);
		data->wstatus = 2;
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return (-1);
	}
	return (0);
}

void	exec_heredoc(t_data *data, t_ast *node)
{
	char	*delim;
	int		pipefd[2];
	int		result;
	int		original_stdin;

	original_stdin = dup(STDIN_FILENO);
	if (validate_heredoc_node(data, node, original_stdin) == -1)
		return ;
	delim = node->right->value;
	if (setup_heredoc_pipe(data, original_stdin, pipefd) == -1)
		return ;
	result = read_heredoc_lines(pipefd[1], delim);
	close(pipefd[1]);
	if (handle_heredoc_result(result, data, pipefd, node) == -1)
	{
		close(pipefd[0]);
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return ;
	}
	close(pipefd[0]);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}
