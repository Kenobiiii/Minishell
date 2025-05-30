/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:42:48 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/30 13:32:23 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_child_redirections(t_data *data)
{
	if (data->input_redir_fd != -1)
	{
		dup2(data->input_redir_fd, STDIN_FILENO);
		close(data->input_redir_fd);
	}
	else if (data->heredoc_pipe_fd != -1)
	{
		dup2(data->heredoc_pipe_fd, STDIN_FILENO);
		close(data->heredoc_pipe_fd);
	}
	if (data->output_redir_fd != -1)
	{
		dup2(data->output_redir_fd, STDOUT_FILENO);
		close(data->output_redir_fd);
	}
}

static void	handle_child(t_data *data, char *path, t_ast *node)
{
	setup_signals_for_child();
	setup_child_redirections(data);
	if (!path)
		exit_minishell(data, node->value, 127);
	if (execve(path, node->args, (char **)list_to_array(data->env)) == -1)
	{
		free(path);
		exit_minishell(data, "Error in execve", 126);
	}
}

void	exec_simple_cmd(t_data *data, t_ast *node)
{
	char	*path;
	pid_t	pid;

	path = get_cmd_path(data, node->value);
	pid = fork();
	if (pid == 0)
		handle_child(data, path, node);
	else if (pid > 0)
	{
		close_redirection_fds(data);
		waitpid(pid, &data->wstatus, 0);
		analyse_status(data);
		free(path);
	}
	else
	{
		free(path);
		close_redirection_fds(data);
		exit_minishell(data, "Error in fork", EXIT_FAILURE);
	}
}

void	exec_ast(t_data *data, t_ast *node)
{
	if (node->type == CMD)
	{
		expand_command_variables(data, node);
		if (is_builtin_command(node->value))
		{
			data->ast = node;
			is_builtins(data, node->value);
		}
		else
			exec_simple_cmd(data, node);
	}
	else if (node->type == REDIRECT_OUT)
		exec_redirect_out(data, node);
	else if (node->type == REDIRECT_IN)
		exec_redirect_in(data, node);
	else if (node->type == REDIN2)
		exec_heredoc(data, node);
	else if (node->type == REDOUT2)
		exec_redirect_append(data, node);
	else if (node->type == PIPE)
		exec_pipe(data, node);
	else if (node->type == AND)
		exec_logical_and(data, node);
	else if (node->type == OR)
		exec_logical_or(data, node);
}
