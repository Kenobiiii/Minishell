/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:14:52 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 15:24:40 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_setup_heredoc_pipe(t_data *data, int pipefd[2], char *delim)
{
	int	result;

	if (!data || !delim || !pipefd)
		return (-1);
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

//? Helper function to collect all heredoc nodes in order
void	collect_heredoc_chain(t_ast *node, t_ast **heredocs, int *count)
{
	if (!node || !heredocs || !count || node->type != REDIN2)
		return ;
	if (node->left && node->left->type == REDIN2)
		collect_heredoc_chain(node->left, heredocs, count);
	if (*count < 10)
		heredocs[(*count)++] = node;
}

static int	process_single_heredoc(t_data *data, t_ast *heredoc, int is_last)
{
	char	*delim;
	int		pipefd[2];

	if (!heredoc || !heredoc->right || !heredoc->right->value)
		return (0);
	delim = heredoc->right->value;
	if (ft_setup_heredoc_pipe(data, pipefd, delim) == -1)
		return (-1);
	if (!is_last)
		close(pipefd[0]);
	else
	{
		if (data->heredoc_pipe_fd != -1)
			close(data->heredoc_pipe_fd);
		data->heredoc_pipe_fd = pipefd[0];
	}
	return (1);
}

void	process_heredoc_loop(t_data *data, t_ast **heredocs, int count)
{
	int	i;

	if (!data || !heredocs || count <= 0)
		return ;
	i = 0;
	while (i < count)
	{
		if (process_single_heredoc(data, heredocs[i], i == count - 1) == -1)
			return ;
		i++;
	}
}

void	exec_heredoc(t_data *data, t_ast *node)
{
	t_ast	*heredocs[10];
	int		count;
	t_ast	*cmd_node;

	if (!data || !node)
		return ;
	count = 0;
	configure_signals(2);
	if (!node->right || !node->right->value)
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
