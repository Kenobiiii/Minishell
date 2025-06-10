/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:02:07 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 15:24:34 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_basic_values(t_data *data)
{
	if (!data)
		return ;
	data->line = NULL;
	data->exit = 0;
	data->prompt = "$Minishell> ";
	data->tokens = NULL;
	data->ast = NULL;
	data->only_redirections = 1;
	data->wstatus = 0;
}

void	init_redirection_fds(t_data *data)
{
	if (!data)
		return ;
	data->input_redir_fd = -1;
	data->output_redir_fd = -1;
	data->heredoc_pipe_fd = -1;
}

void	close_redirection_fds(t_data *data)
{
	if (!data)
		return ;
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
