/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:27:38 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/30 14:42:01 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void exec_redirect_out(t_data *data, t_ast *node)
{
	int fd;
	char *filename;

	filename = node->right->value;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	exec_ast(data, node->left);
}

void exec_redirect_in(t_data *data, t_ast *node)
{
	int fd;
	char *filename;

	filename = node->right->value;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		return;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	exec_ast(data, node->left);
}

void exec_redirect_append(t_data *data, t_ast *node)
{
	int fd;
	char *filename;

	filename = node->right->value;
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return;
	} 
	dup2(fd, STDOUT_FILENO);
	close(fd);
	exec_ast(data, node->left);
}

void	exec_heredoc(t_ast *node)
{
	char	*delim;
	int		pipefd[2];
	char	*line;

	if (!node->right || !node->right->value) {
		ft_putstr_fd("minishell: syntax error: missing delimiter for <<\n", STDERR_FILENO);
		return;
	}
	delim = node->right->value;
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return;
	}
	while (1)
	{
		line = readline("here_doc> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", STDERR_FILENO);
			break;
		}
		if (strncmp(line, delim, strlen(delim)) == 0 && (line[strlen(delim)] == '\0')) 
		{
			free(line);
			break;
		}
		if (write(pipefd[1], line, strlen(line)) == -1 || write(pipefd[1], "\n", 1) == -1)
		{
			perror("minishell");
			free(line);
			break;
		}
		free(line);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		perror("minishell");
	close(pipefd[0]);
}
