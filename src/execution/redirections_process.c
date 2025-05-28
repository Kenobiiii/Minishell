/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:10:00 by paromero          #+#    #+#             */
/*   Updated: 2025/05/28 16:49:25 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

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
