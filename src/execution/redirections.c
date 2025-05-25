/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:27:38 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/21 17:29:52 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Necesitamos g_shell_state para el manejo de SIGINT en heredoc
extern volatile sig_atomic_t g_shell_state;

// Encuentra el comando real detrás de las redirecciones
static t_ast	*find_cmd_node(t_ast *node)
{
	t_ast	*current;

	if (!node)
		return (NULL);
	current = node;
	while (current && (current->type == REDIRECT_OUT
			|| current->type == REDOUT2))
		current = current->left;
	return (current);
}

// Encuentra la última redirección en una cadena
static t_ast	*find_last_redirection(t_ast *node)
{
	if (!node || (node->type != REDIRECT_OUT && node->type != REDOUT2))
		return (NULL);
	return (node);
}

static int	check_file_permissions(t_ast *current, t_data *data)
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

static void	apply_last_redirect(t_ast *redirect, t_ast *cmd, t_data *data)
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

void	exec_redirect_out(t_data *data, t_ast *node)
{
	t_ast	*cmd_node;
	t_ast	*last_redirect;
	int		original_stdout;

	if (!node || !node->right)
	{
		ft_printf("No hay archivo de salida\n");
		data->wstatus = 2;
		return ;
	}
	original_stdout = dup(STDOUT_FILENO);
	cmd_node = find_cmd_node(node);
	last_redirect = find_last_redirection(node);
	if (check_file_permissions(node, data))
		apply_last_redirect(last_redirect, cmd_node, data);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
}

void	exec_redirect_append(t_data *data, t_ast *node)
{
	t_ast	*cmd_node;
	t_ast	*last_redirect;
	int		original_stdout;

	if (!node || !node->right)
	{
		ft_printf("No hay archivo de salida\n");
		data->wstatus = 2;
		return ;
	}
	original_stdout = dup(STDOUT_FILENO);
	cmd_node = find_cmd_node(node);
	last_redirect = find_last_redirection(node);
	if (check_file_permissions(node, data))
		apply_last_redirect(last_redirect, cmd_node, data);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
}

static int	read_heredoc_lines_part2(int pipefd, char *line)
{
	if (write(pipefd, line, strlen(line)) == -1
		|| write(pipefd, "\n", 1) == -1)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (1);
}

static int	read_heredoc_lines(int pipefd, char *delim)
{
	char	*line;
	int		result;

	if (!delim)
	{
		ft_putstr_fd("error: delimiter is NULL\n", STDERR_FILENO);
		return (-1);
	}

	while (1)
	{
		// Si SIGINT ocurrió ANTES de llamar a readline (por ejemplo, procesando la línea anterior)
		if (g_shell_state == STATE_EXECUTION_INTERRUPTED)
		{
			// No cambiar a HEREDOC_INTERRUPTED aquí, porque readline no ha sido llamado aún
			// y el handler ya puso el estado correcto para una interrupción de ejecución general.
			return (-1); 
		}

		line = readline("> ");
		
		// CASO 1: readline fue interrumpido por SIGINT (devuelve NULL) O Ctrl+D (devuelve NULL)
		if (!line)
		{
			if (g_shell_state == STATE_EXECUTION_INTERRUPTED) // Fue SIGINT quien interrumpió readline
			{
				g_shell_state = STATE_HEREDOC_INTERRUPTED; // Cambiamos al estado específico
				return (-1); // Interrupción por SIGINT en heredoc
			}
			else // Fue Ctrl+D (EOF)
			{
				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
				ft_putstr_fd(delim, STDERR_FILENO);
				ft_putstr_fd("`)\n", STDERR_FILENO);
				return (0); // EOF, no es una interrupción SIGINT
			}
		}

		// CASO 2: SIGINT ocurrió MIENTRAS readline estaba activo, pero readline NO devolvió NULL (menos común)
		// o si la señal se procesó justo después de que readline retornara con una línea válida.
		if (g_shell_state == STATE_EXECUTION_INTERRUPTED)
		{
			g_shell_state = STATE_HEREDOC_INTERRUPTED; // Cambiamos al estado específico
			free(line);
			return (-1);
		}
		
		if (strlen(line) == strlen(delim) 
			&& strncmp(line, delim, strlen(delim)) == 0)
		{
			free(line);
			break ; // Delimitador encontrado
		}
		
		result = read_heredoc_lines_part2(pipefd, line); // line es liberado dentro de part2
		if (result < 0) // Error de escritura
			return (result);
	}
	return (1); // Éxito, delimitador encontrado y todo escrito
}

void	exec_redirect_in(t_data *data, t_ast *node)
{
	int		fd;
	char	*filename;
	int		original_stdin;

	original_stdin = dup(STDIN_FILENO);
	if (!node->right)
	{
		ft_printf("No hay archivo de entrada\n");
		data->wstatus = 2;
		return ;
	}
	filename = node->right->value;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return ;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	exec_ast(data, node->left);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}

void	exec_heredoc(t_data *data, t_ast *node)
{
	char	*delim;
	int		pipefd[2];
	int		result;
	int		original_stdin;

	if (!node)
	{
		ft_putstr_fd("error: heredoc node is NULL\n", STDERR_FILENO);
		data->wstatus = 1;
		return ;
	}
	
	original_stdin = dup(STDIN_FILENO);
	
	if (!node->right || !node->right->value)
	{
		ft_putstr_fd("syntax error: missing delimiter for <<\n", STDERR_FILENO);
		data->wstatus = 2;
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return ;
	}
	
	delim = node->right->value;
	
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		data->wstatus = 1;
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return ;
	}
	
	// g_shell_state es STATE_EXECUTING_COMMAND aquí, establecido por main.c
	result = read_heredoc_lines(pipefd[1], delim);
	close(pipefd[1]);
	
	// Si read_heredoc_lines retornó -1, g_shell_state es STATE_SIGINT_RECEIVED_IN_EXECUTION
	if (result == -1) // Interrumpido por SIGINT
	{
		data->wstatus = 130; // Cancelled by SIGINT
		// main.c se encargará de resetear g_shell_state y redibujar el prompt.
	}
	else if (result == 0) // EOF (Ctrl+D)
	{
		// Manejar EOF como sea apropiado, quizás un error o continuar sin entrada.
		// data->wstatus podría necesitar ajustarse.
	}
	else // result == 1 (éxito)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			data->wstatus = 1;
		}
		else if (node->left)
		{
			exec_ast(data, node->left);
		}
	}
	
	close(pipefd[0]);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	// Al salir de aquí, main.c eventualmente reseteará g_shell_state a STATE_PROMPT_NORMAL
	// o manejará STATE_SIGINT_RECEIVED_IN_EXECUTION.
}
