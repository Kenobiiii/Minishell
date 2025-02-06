/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:42:48 by anggalle          #+#    #+#             */
/*   Updated: 2025/02/06 14:14:51 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void exec_logical_or(t_data *data, t_ast *node)
{
	exec_ast(data, node->left);
	if (WEXITSTATUS(data->wstatus) != 0)
	{
		exec_ast(data, node->right);
	}
}
void exec_logical_and(t_data *data, t_ast *node)
{
	exec_ast(data, node->left);
	if (WEXITSTATUS(data->wstatus) == 0)
	{
		exec_ast(data, node->right);
	}
}

void	analyse_status(t_data *data)
{
	int status;

	status = data->wstatus;
	if (WIFEXITED(status))
	{
		data->wstatus = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		data->wstatus = 128 + WTERMSIG(status);
	}
	// else if (WIFSTOPPED(status))
	// {
	// 	data->wstatus = 128 + WSTOPSIG(status);
	// }
	else
	{
		data->wstatus = status;
	}
}

void exec_simple_cmd(t_data *data, t_ast *node)
{
	char *path;
	pid_t pid;

	path = get_cmd_path(data, node->value);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (!path)
		{
			printf("%s: command not found\n", node->value);
			exit(127);
		}
		else if (execve(path, node->args, (char **)list_to_array(data->env)) == -1)
		{
			free(path);
			perror("Error en execve");
			exit(126);
		}
	} else if (pid > 0)
	{
		waitpid(pid, &data->wstatus, 0);
		analyse_status(data);
		free(path);
	} else
	{
		free(path);
		perror("Error en fork");
	}
}

void exec_ast(t_data *data, t_ast *node)
{
	if (!node)
		return;
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

void exec_func(t_data *data)
{
	exec_ast(data, data->ast);
}
