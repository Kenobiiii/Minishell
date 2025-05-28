/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:59:00 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/28 17:42:23 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Executes the left side of a pipe
 * Redirects standard output to the write end of the pipe
 */
static void	exec_pipe_left(t_data *data, t_ast *node, int pipefd[2])
{
	setup_signals_for_child();
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		free_minishell(data);
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	exec_ast(data, node->left);
	free_minishell(data);
	exit(data->wstatus);
}

/**
 * Executes the right side of a pipe
 * Redirects standard input to the read end of the pipe
 * Correctly handles redirections if present
 */
static void	exec_pipe_right(t_data *data, t_ast *node, int pipefd[2])
{
	setup_signals_for_child();
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		free_minishell(data);
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	if (node->right && (node->right->type == REDIRECT_OUT
			|| node->right->type == REDOUT2))
		exec_ast(data, node->right);
	else
		exec_ast(data, node->right);
	free_minishell(data);
	exit(data->wstatus);
}

/**
 * Waits for child processes to finish and updates the exit status
 * Only the exit status of the last command (right side) is considered
 */
static void	wait_for_processes(t_data *data, pid_t pid_left, pid_t pid_right)
{
	int	status_left;
	int	status_right;

	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	if (WIFEXITED(status_right))
		data->wstatus = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
		data->wstatus = 128 + WTERMSIG(status_right);
}

/**
 * Handles creation and execution of processes for each side of the pipe
 */
static void	handle_pipe_processes(t_data *data, t_ast *node, int pipefd[2])
{
	pid_t	pid_left;
	pid_t	pid_right;

	pid_left = handle_process_error(fork(), "fork");
	if (pid_left == 0)
		exec_pipe_left(data, node, pipefd);
	else if (pid_left < 0)
		return ;
	pid_right = handle_process_error(fork(), "fork");
	if (pid_right == 0)
		exec_pipe_right(data, node, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	wait_for_processes(data, pid_left, pid_right);
}

/**
 * Creates a pipe and executes the commands connected by it
 */
void	exec_pipe(t_data *data, t_ast *node)
{
	int		pipefd[2];

	if (handle_process_error(pipe(pipefd), "pipe") < 0)
		return ;
	handle_pipe_processes(data, node, pipefd);
}
