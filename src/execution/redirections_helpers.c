/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/03 10:30:07 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

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
