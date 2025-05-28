/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:14:52 by paromero          #+#    #+#             */
/*   Updated: 2025/05/28 16:33:28 by paromero         ###   ########.fr       */
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

static int	validate_heredoc_input(t_data *data, t_ast *node)
{
	if (!node || !node->right || !node->right->value)
	{
		ft_putstr_fd("syntax error: missing delimiter for <<\n", STDERR_FILENO);
		data->wstatus = 2;
		return (-1);
	}
	return (0);
}

static int	create_heredoc_pipe(t_data *data, int pipefd[2])
{
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		data->wstatus = 1;
		return (-1);
	}
	return (0);
}

static int	process_heredoc_content(t_data *data, int pipefd[2], char *delim)
{
	int	result;

	result = read_heredoc_lines(pipefd[1], delim);
	close(pipefd[1]);
	if (result == -1)
	{
		close(pipefd[0]);
		data->wstatus = 130;
		return (-1);
	}
	return (0);
}

void	exec_heredoc(t_data *data, t_ast *node)
{
	char	*delim;
	int		pipefd[2];

	if (validate_heredoc_input(data, node) == -1)
		return ;
	delim = node->right->value;
	if (create_heredoc_pipe(data, pipefd) == -1)
		return ;
	if (process_heredoc_content(data, pipefd, delim) == -1)
		return ;
	if (data->heredoc_pipe_fd != -1)
		close(data->heredoc_pipe_fd);
	data->heredoc_pipe_fd = pipefd[0];
	if (node->left)
		exec_ast(data, node->left);
}
