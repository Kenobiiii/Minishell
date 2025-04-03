/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:27:38 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/29 18:01:05 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_redirect_out(t_data *data, t_ast *node)
{
	int		fd;
	char	*filename;
	int		original_stdout;

	original_stdout = dup(STDOUT_FILENO);
	if (!node->right)
	{
		ft_printf("No hay archivo de salida\n");
		data->wstatus = 2;
		return ;
	}
	filename = node->right->value;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	exec_ast(data, node->left);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
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

void	exec_redirect_append(t_data *data, t_ast *node)
{
	int		fd;
	char	*filename;
	int		original_stdout;

	original_stdout = dup(STDOUT_FILENO);
	filename = node->right->value;
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	exec_ast(data, node->left);
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
