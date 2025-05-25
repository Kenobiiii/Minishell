/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:42:48 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/26 17:52:45 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_child(t_data *data, char *path, t_ast *node)
{
	setup_signals_for_child();
	if (!path)
		exit_minishell(data, "command not found", 127);
	if (execve(path, node->args, (char **)list_to_array(data->env)) == -1)
	{
		free(path);
		exit_minishell(data, "Error in execve", 126);
	}
}

void	exec_simple_cmd(t_data *data, t_ast *node)
{
	char	*path;
	pid_t	pid;

	path = get_cmd_path(data, node->value);
	pid = fork();
	if (pid == 0)
		handle_child(data, path, node);
	else if (pid > 0)
	{
		waitpid(pid, &data->wstatus, 0);
		analyse_status(data);
		free(path);
	}
	else
	{
		free(path);
		exit_minishell(data, "Error in fork", EXIT_FAILURE);
	}
}

void	exec_ast(t_data *data, t_ast *node)
{
	if (!node)
		return ;
	if (node->type == CMD)
		exec_simple_cmd(data, node);
	else if (node->type == REDIRECT_OUT)
		exec_redirect_out(data, node);
	else if (node->type == REDIRECT_IN)
		exec_redirect_in(data, node);
	else if (node->type == REDIN2)
		exec_heredoc(data, node);
	else if (node->type == REDOUT2)
		exec_redirect_append(data, node);
	else if (node->type == PIPE)
		exec_pipe(data, node);
	else if (node->type == AND)
		exec_logical_and(data, node);
	else if (node->type == OR)
		exec_logical_or(data, node);
}

void	exec_func(t_data *data)
{
	exec_ast(data, data->ast);
}
