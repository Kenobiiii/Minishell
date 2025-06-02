/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:42:48 by anggalle          #+#    #+#             */
/*   Updated: 2025/06/02 17:33:46 by paromero         ###   ########.fr       */
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
	configure_signals(1);
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

static void	exec_command_node(t_data *data, t_ast *node)
{
	t_ast	*original_ast;

	expand_command_variables(data, node);
	if (is_builtin_command(node->value))
	{
		original_ast = data->ast;
		data->ast = node;
		execute_builtin_with_redirections(data);
		data->ast = original_ast;
	}
	else
		exec_simple_cmd(data, node);
}

void	exec_ast(t_data *data, t_ast *node)
{
	if (node->type == CMD)
		exec_command_node(data, node);
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
}
