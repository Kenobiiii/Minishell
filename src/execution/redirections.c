/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:27:38 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/14 19:11:43 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	exec_redirect_out(t_data *data, t_ast *node)
{
	int		fd;
	char	*filename;
	t_ast	*current;
	t_ast	*cmd_node;
	t_ast	*last_redirect;
	int		original_stdout;
	int		error_occurred = 0;

	original_stdout = dup(STDOUT_FILENO);
	if (!node || !node->right)
	{
		ft_printf("No hay archivo de salida\n");
		data->wstatus = 2;
		return;
	}
	
	// Encontrar el comando real
	cmd_node = find_cmd_node(node);
	
	// Encontrar la última redirección para aplicar el contenido
	last_redirect = find_last_redirection(node);
	
	// Comprobamos permisos para todos los archivos primero
	// Si alguno falla, no se crea ninguno
	current = node;
	while (current && (current->type == REDIRECT_OUT || current->type == REDOUT2))
	{
		if (current->right)
		{
			filename = current->right->value;
			// Solo comprobar si podemos abrir/crear el archivo
			int flags = current->type == REDOUT2 ? 
				O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC;
			
			fd = open(filename, flags, 0644);
			if (fd == -1)
			{
				perror("minishell");
				data->wstatus = 1;
				error_occurred = 1;
				close(original_stdout);
				return; // Si falla algún archivo, salimos completamente
			}
			close(fd);
		}
		current = current->left;
	}
	
	// Si todos los archivos pasan la comprobación, abrimos el último
	// y ejecutamos el comando
	if (!error_occurred)
	{
		if (last_redirect && last_redirect->right)
		{
			filename = last_redirect->right->value;
			int flags = last_redirect->type == REDOUT2 ? 
				O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC;
			
			fd = open(filename, flags, 0644);
			if (fd != -1)
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
				
				if (cmd_node)
					exec_ast(data, cmd_node);
			}
		}
	}
	
	// Restaurar stdout
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
}

void	exec_redirect_append(t_data *data, t_ast *node)
{
	int		fd;
	char	*filename;
	t_ast	*current;
	t_ast	*cmd_node;
	t_ast	*last_redirect;
	int		original_stdout;
	int		error_occurred = 0;

	original_stdout = dup(STDOUT_FILENO);
	if (!node || !node->right)
	{
		ft_printf("No hay archivo de salida\n");
		data->wstatus = 2;
		return;
	}
	
	// Encontrar el comando real
	cmd_node = find_cmd_node(node);
	
	// Encontrar la última redirección para aplicar el contenido
	last_redirect = find_last_redirection(node);
	
	// Comprobamos permisos para todos los archivos primero
	// Si alguno falla, no se crea ninguno
	current = node;
	while (current && (current->type == REDIRECT_OUT || current->type == REDOUT2))
	{
		if (current->right)
		{
			filename = current->right->value;
			// Solo comprobar si podemos abrir/crear el archivo
			int flags = current->type == REDOUT2 ? 
				O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC;
			
			fd = open(filename, flags, 0644);
			if (fd == -1)
			{
				perror("minishell");
				data->wstatus = 1;
				error_occurred = 1;
				close(original_stdout);
				return; // Si falla algún archivo, salimos completamente
			}
			close(fd);
		}
		current = current->left;
	}
	
	// Si todos los archivos pasan la comprobación, abrimos el último
	// y ejecutamos el comando
	if (!error_occurred)
	{
		if (last_redirect && last_redirect->right)
		{
			filename = last_redirect->right->value;
			int flags = last_redirect->type == REDOUT2 ? 
				O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC;
			
			fd = open(filename, flags, 0644);
			if (fd != -1)
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
				
				if (cmd_node)
					exec_ast(data, cmd_node);
			}
		}
	}
	
	// Restaurar stdout
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
}

static int	read_heredoc_lines(int pipefd, char *delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("here-document delimited by EOF\n", STDERR_FILENO);
			return (0);
		}
		if (strncmp(line, delim, strlen(delim)) == 0
			&& (line[strlen(delim)] == '\0'))
		{
			free(line);
			break ;
		}
		if (write(pipefd, line, strlen(line)) == -1
			|| write(pipefd, "\n", 1) == -1)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (1);
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

	original_stdin = dup(STDIN_FILENO);
	if (!node->left)
	{
		ft_putstr_fd("syntax error: missing delimiter for <<\n", STDERR_FILENO);
		return ;
	}
	delim = node->left->value;
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return ;
	}
	result = read_heredoc_lines(pipefd[1], delim);
	close(pipefd[1]);
	if (result >= 0 && dup2(pipefd[0], STDIN_FILENO) == -1)
		perror("minishell");
	close(pipefd[0]);
	exec_ast(data, node->right);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}
