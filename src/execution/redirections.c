/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:27:38 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/26 22:14:52 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

int	write_heredoc_line(int pipefd, char *line)
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

t_ast	*find_cmd_node(t_ast *node)
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
