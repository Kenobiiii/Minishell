/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:59:00 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/14 19:10:25 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Ejecuta el lado izquierdo de una tubería
 * Redirige la salida estándar al extremo de escritura de la tubería
 */
static void	exec_pipe_left(t_data *data, t_ast *node, int pipefd[2])
{
	setup_signals_for_child();
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

/**
 * Ejecuta el lado derecho de una tubería
 * Redirige la entrada estándar al extremo de lectura de la tubería
 * Maneja correctamente las redirecciones si están presentes
 */
static void	exec_pipe_right(t_data *data, t_ast *node, int pipefd[2])
{
	setup_signals_for_child();
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	if (node->right && (node->right->type == REDIRECT_OUT
			|| node->right->type == REDOUT2))
		exec_ast(data, node->right);
	else
		exec_ast(data, node->right);
	exit(data->wstatus);
}

/**
 * Espera a que terminen los procesos hijo y actualiza el estado de salida
 * Solo se considera el estado de salida del último comando (derecho)
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
 * Maneja la creación y ejecución de los procesos para cada lado de la tubería
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
 * Crea una tubería y ejecuta los comandos conectados por ella
 */
void	exec_pipe(t_data *data, t_ast *node)
{
	int		pipefd[2];

	if (handle_process_error(pipe(pipefd), "pipe") < 0)
		return ;
	handle_pipe_processes(data, node, pipefd);
}
