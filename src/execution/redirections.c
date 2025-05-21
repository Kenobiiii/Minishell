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
		result = read_heredoc_lines_part2(pipefd, line);
		if (result < 0)
			return (result);
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
