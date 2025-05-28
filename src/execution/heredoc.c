/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:14:52 by paromero          #+#    #+#             */
/*   Updated: 2025/05/26 22:14:52 by paromero         ###   ########.fr       */
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

void	exec_heredoc(t_data *data, t_ast *node)
{
	char	*delim;
	int		pipefd[2];
	int		result;

	if (!node || !node->right || !node->right->value)
	{
		ft_putstr_fd("syntax error: missing delimiter for <<\n", STDERR_FILENO);
		data->wstatus = 2;
		return ;
	}
	
	delim = node->right->value;
	
	// Crear el pipe para el heredoc
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		data->wstatus = 1;
		return ;
	}
	
	// Leer las líneas del heredoc y escribirlas al pipe
	result = read_heredoc_lines(pipefd[1], delim);
	close(pipefd[1]);  // Cerrar el extremo de escritura
	
	if (result == -1)
	{
		close(pipefd[0]);
		data->wstatus = 130;
		return ;
	}
	
	// Cerrar el descriptor anterior si existía
	if (data->heredoc_pipe_fd != -1)
		close(data->heredoc_pipe_fd);
	
	// Guardar el extremo de lectura del pipe para el proceso hijo
	data->heredoc_pipe_fd = pipefd[0];
	
	// Ejecutar recursivamente el comando de la izquierda
	if (node->left)
		exec_ast(data, node->left);
}
