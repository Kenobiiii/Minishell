/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:21:25 by paromero          #+#    #+#             */
/*   Updated: 2025/05/28 17:27:02 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Resets redirection file descriptors to -1
 * without closing them (assumes they were already closed or transferred)
 */
void	reset_redirection_fds(t_data *data)
{
	data->input_redir_fd = -1;
	data->output_redir_fd = -1;
	data->heredoc_pipe_fd = -1;
}

/**
 * Closes all open redirection file descriptors
 * and resets them to -1
 */
void	close_redirection_fds(t_data *data)
{
	if (data->input_redir_fd != -1)
	{
		close(data->input_redir_fd);
		data->input_redir_fd = -1;
	}
	if (data->output_redir_fd != -1)
	{
		close(data->output_redir_fd);
		data->output_redir_fd = -1;
	}
	if (data->heredoc_pipe_fd != -1)
	{
		close(data->heredoc_pipe_fd);
		data->heredoc_pipe_fd = -1;
	}
}

/**
 * Applies redirections for built-ins (which don't fork)
 * Returns 1 if redirections were applied, 0 if not
 */
int	apply_redirections_for_builtin(t_data *data)
{
	int	redirections_applied;

	redirections_applied = 0;
	if (data->input_redir_fd != -1)
	{
		dup2(data->input_redir_fd, STDIN_FILENO);
		redirections_applied = 1;
	}
	else if (data->heredoc_pipe_fd != -1)
	{
		dup2(data->heredoc_pipe_fd, STDIN_FILENO);
		redirections_applied = 1;
	}
	if (data->output_redir_fd != -1)
	{
		dup2(data->output_redir_fd, STDOUT_FILENO);
		redirections_applied = 1;
	}
	return (redirections_applied);
}

/**
 *? Restaura stdin y stdout a sus valores originales,
 *? after executing a built-in
 */
void	restore_redirections_for_builtin(t_data *data,
	int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close_redirection_fds(data);
}
