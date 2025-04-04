/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:59:00 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/29 01:54:38 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_process_error(int value, const char *msg)
{
	if (value < 0)
	{
		perror(msg);
		return (-1);
	}
	return (value);
}

static void	exec_pipe_left(t_data *data, t_ast *node, int pipefd[2])
{
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	exec_ast(data, node->left);
	exit(data->wstatus);
}

static void	exec_pipe_right(t_data *data, t_ast *node, int pipefd[2])
{
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	exec_ast(data, node->right);
	exit(data->wstatus);
}

void	exec_pipe(t_data *data, t_ast *node)
{
    int		pipefd[2];
    pid_t	pid_left;
    pid_t	pid_right;
    int		status_left;
    int		status_right;

    if (handle_process_error(pipe(pipefd), "pipe") < 0)
        return ;
    pid_left = handle_process_error(fork(), "fork");
    if (pid_left == 0)
        exec_pipe_left(data, node, pipefd);
    else if (pid_left < 0)
        return ;
    pid_right = handle_process_error(fork(), "fork");
    if (pid_right == 0)
        exec_pipe_right(data, node, pipefd);
    else if (pid_right < 0)
        return ;
    close(pipefd[0]);
    close(pipefd[1]);

    /* Esperar a ambos procesos hijos */
    waitpid(pid_left, &status_left, 0);
    waitpid(pid_right, &status_right, 0);

    /* Se asigna el exit status del proceso derecho */
    if (WIFEXITED(status_right))
        data->wstatus = WEXITSTATUS(status_right);
    else if (WIFSIGNALED(status_right))
        data->wstatus = 128 + WTERMSIG(status_right);
}

