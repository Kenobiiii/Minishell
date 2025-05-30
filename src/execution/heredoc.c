/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:14:52 by paromero          #+#    #+#             */
/*   Updated: 2025/05/30 13:35:23 by paromero         ###   ########.fr       */
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

static int	setup_heredoc_pipe(t_data *data, int pipefd[2], char *delim)
{
	int	result;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		data->wstatus = 1;
		return (-1);
	}
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

// Helper function to collect all heredoc nodes in order
static void	collect_heredoc_chain(t_ast *node, t_ast **heredocs, int *count)
{
	if (!node || node->type != REDIN2)
		return ;
	if (node->left && node->left->type == REDIN2)
		collect_heredoc_chain(node->left, heredocs, count);
	heredocs[(*count)++] = node;
}

static void	process_heredoc_loop(t_data *data, t_ast **heredocs, int count)
{
	int		i;
	char	*delim;
	int		pipefd[2];

	i = 0;
	while (i < count)
	{
		delim = heredocs[i]->right->value;
		if (setup_heredoc_pipe(data, pipefd, delim) == -1)
			return ;
		if (i < count - 1)
			close(pipefd[0]);
		else
		{
			if (data->heredoc_pipe_fd != -1)
				close(data->heredoc_pipe_fd);
			data->heredoc_pipe_fd = pipefd[0];
		}
		i++;
	}
}

void	exec_heredoc(t_data *data, t_ast *node)
{
	t_ast	*heredocs[10];
	int		count;
	t_ast	*cmd_node;

	count = 0;
	if (!node || !node->right || !node->right->value)
	{
		ft_putstr_fd("syntax error: missing delimiter for <<\n", STDERR_FILENO);
		data->wstatus = 2;
		return ;
	}
	collect_heredoc_chain(node, heredocs, &count);
	process_heredoc_loop(data, heredocs, count);
	cmd_node = node;
	while (cmd_node && cmd_node->type == REDIN2)
		cmd_node = cmd_node->left;
	if (cmd_node)
		exec_ast(data, cmd_node);
}
