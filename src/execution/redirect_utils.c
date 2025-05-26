/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:14:52 by paromero          #+#    #+#             */
/*   Updated: 2025/05/26 22:18:04 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_file_permissions(t_ast *current, t_data *data)
{
	int		fd;
	int		flags;
	char	*filename;

	while (current && (current->type == REDIRECT_OUT
			|| current->type == REDOUT2))
	{
		if (current->right)
		{
			filename = current->right->value;
			flags = O_WRONLY | O_CREAT;
			if (current->type == REDOUT2)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(filename, flags, 0644);
			if (fd == -1)
				fd_error(data);
			close(fd);
		}
		current = current->left;
	}
	return (1);
}

void	apply_last_redirect(t_ast *redirect, t_ast *cmd, t_data *data)
{
	int		fd;
	int		flags;
	char	*filename;

	if (!redirect || !redirect->right)
		return ;
	filename = redirect->right->value;
	flags = O_WRONLY | O_CREAT;
	if (redirect->type == REDOUT2)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd != -1)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		if (cmd)
			exec_ast(data, cmd);
	}
}

int	setup_stdin_and_exec(int pipefd[2], t_ast *node, t_data *data)
{
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		data->wstatus = 1;
		return (-1);
	}
	if (node->left)
		exec_ast(data, node->left);
	return (0);
}

int	setup_heredoc_pipe(t_data *data, int original_stdin, int pipefd[2])
{
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		data->wstatus = 1;
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return (-1);
	}
	return (0);
}

int	handle_heredoc_result(int result, t_data *data, int pipefd[2], t_ast *node)
{
	if (result == -1)
	{
		data->wstatus = 130;
		return (-1);
	}
	else if (result == 0)
	{
		data->wstatus = 0;
		return (setup_stdin_and_exec(pipefd, node, data));
	}
	else
	{
		return (setup_stdin_and_exec(pipefd, node, data));
	}
}
