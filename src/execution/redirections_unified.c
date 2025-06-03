/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_unified.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/03 10:25:17 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

/* ====================== UTILITY FUNCTIONS ====================== */

int	write_heredoc_line(int pipefd, char *line)
{
	if (write(pipefd, line, ft_strlen(line)) == -1
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

int	count_output_redirections(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == REDIRECT_OUT || node->type == REDOUT2)
		return (1 + count_output_redirections(node->left));
	return (0);
}

void	collect_redirections(t_ast *node, t_ast **redirections, int *index)
{
	if (!node)
		return ;
	if (node->type == REDIRECT_OUT || node->type == REDOUT2)
	{
		collect_redirections(node->left, redirections, index);
		redirections[(*index)++] = node;
	}
}

int	open_redirection_file(t_ast *redirection)
{
	int	fd;

	if (redirection->type == REDIRECT_OUT)
		fd = open(redirection->right->value, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	else
		fd = open(redirection->right->value, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	return (fd);
}

/* ====================== VALIDATION FUNCTIONS ====================== */

static int	validate_redirect_node(t_data *data, t_ast *node)
{
	if (!node || !node->right)
	{
		ft_printf("No hay archivo de salida\n");
		data->wstatus = 2;
		return (-1);
	}
	return (0);
}

int	validate_input_redirect_node(t_data *data, t_ast *node)
{
	if (!node->right)
	{
		ft_printf("No hay archivo de entrada\n");
		data->wstatus = 2;
		return (-1);
	}
	return (0);
}

/* ====================== FILE HANDLING FUNCTIONS ====================== */

static int	handle_output_redirect_open(t_data *data, char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return (-1);
	}
	if (data->output_redir_fd != -1)
		close(data->output_redir_fd);
	data->output_redir_fd = fd;
	return (0);
}

int	handle_input_redirect_open(t_data *data, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return (-1);
	}
	if (data->input_redir_fd != -1)
		close(data->input_redir_fd);
	data->input_redir_fd = fd;
	return (0);
}

/* ====================== PROCESSING FUNCTIONS ====================== */

static int	handle_redirection_fd(t_data *data, int fd, int i, int count)
{
	if (i < count - 1)
		close(fd);
	else
	{
		if (data->output_redir_fd != -1)
			close(data->output_redir_fd);
		data->output_redir_fd = fd;
	}
	return (0);
}

static int	process_single_redirection(t_data *data, t_ast *redirection,
			int i, int count)
{
	int	fd;

	if (!redirection->right)
		return (0);
	fd = open_redirection_file(redirection);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return (-1);
	}
	return (handle_redirection_fd(data, fd, i, count));
}

int	process_redirection_files(t_data *data, t_ast **redirections, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (process_single_redirection(data, redirections[i], i, count) == -1)
			return (-1);
		i++;
	}
	return (0);
}

void	execute_command_after_redirections(t_data *data, t_ast *node)
{
	t_ast	*cmd_node;

	cmd_node = node;
	while (cmd_node && (cmd_node->type == REDIRECT_OUT
			|| cmd_node->type == REDOUT2))
		cmd_node = cmd_node->left;
	if (cmd_node)
		exec_ast(data, cmd_node);
}

/* ====================== MULTIPLE REDIRECTIONS FUNCTIONS ====================== */

static int	handle_single_redirection(t_data *data, t_ast *node)
{
	if (node->type == REDIRECT_OUT)
		exec_redirect_out(data, node);
	else if (node->type == REDOUT2)
		exec_redirect_append(data, node);
	return (1);
}

static t_ast	**allocate_redirections_array(int count)
{
	t_ast	**redirections;

	redirections = malloc(sizeof(t_ast *) * count);
	return (redirections);
}

static void	process_redirections_array(t_data *data, t_ast *node, int count)
{
	t_ast	**redirections;
	int		index;

	redirections = allocate_redirections_array(count);
	if (!redirections)
		return ;
	index = 0;
	collect_redirections(node, redirections, &index);
	if (process_redirection_files(data, redirections, count) == -1)
	{
		free(redirections);
		return ;
	}
	free(redirections);
	execute_command_after_redirections(data, node);
}

void	process_multiple_output_redirections(t_data *data, t_ast *node)
{
	int	count;

	count = count_output_redirections(node);
	if (count <= 1)
	{
		handle_single_redirection(data, node);
		return ;
	}
	process_redirections_array(data, node, count);
}

/* ====================== MAIN HELPER FUNCTION ====================== */

void	execute_redirect_helper(t_data *data, t_ast *node, int flags)
{
	char	*filename;

	if (validate_redirect_node(data, node) == -1)
		return ;
	if (count_output_redirections(node) > 1)
	{
		process_multiple_output_redirections(data, node);
		return ;
	}
	filename = node->right->value;
	if (handle_output_redirect_open(data, filename, flags) == -1)
		return ;
	if (node->left)
		exec_ast(data, node->left);
}
