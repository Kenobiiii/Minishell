/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:42:48 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/21 11:48:17 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_command(char **cmd, char *argv)
{
	cmd[0] = "/bin/sh";
	cmd[1] = "-c";
	cmd[2] = argv;
	cmd[3] = NULL;
}

void analyse_status(int wstatus)
{
	if (WIFSTOPPED(wstatus))
	{
		ft_printf("Proceso suspendido\n");
	}else if (WIFCONTINUED(wstatus))
	{
		ft_printf("Proceso continuado\n");
	}else if (WIFSIGNALED(wstatus))
	{
		ft_printf("Proceso parado por una señal\n");
	}else
	{
		ft_printf("El proceso finalizó correctamente\n");
	}
}

void exec_func(t_data *data)
{
	int pid_fork;
	int pid_wait;
	int wstatus;
	char	*path = ft_strcat("/bin/", data->ast->value);

	pid_fork = fork();
	if (pid_fork < 0)
	{
		perror("Error en el fork");
		exit(-1);
	}
	else if (pid_fork == 0) //proceso hijo
	{
		execve(path, data->ast->args, NULL);
		perror("Errror en el execve");
		exit(-1);
	}
	else if (pid_fork > 0) //Proceso padre
	{
		pid_wait = waitpid(pid_fork, &wstatus, WUNTRACED);
		if (pid_wait < 0)
		{
			perror("Error en el waitpid");
			exit(-1);
		}
		analyse_status(wstatus);
	}
}
