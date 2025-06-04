/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:09:01 by anggalle          #+#    #+#             */
/*   Updated: 2025/04/09 18:01:02 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_putstatus_fd(char *status, int fd)
{
	int	i;

	i = 0;
	if (status != NULL)
	{
		while (status[i])
			write (fd, &status[i++], 1);
	}
	free (status);
}

static int	check_n_option(char *arg)
{
	int	j;

	if (ft_strncmp(arg, "-n", 2) != 0)
		return (0);
	if (ft_strncmp(arg, "-n", ft_strlen(arg)) == 0)
		return (1);
	j = 2;
	while (arg[j] == 'n')
		j++;
	if (arg[j])
		return (0);
	return (1);
}

static void	print_arg(t_data *data, char *arg, int has_next)
{
	if (ft_strncmp(arg, "$?", 2) == 0)
		ft_putstatus_fd(ft_itoa(data->wstatus), STDOUT_FILENO);
	else
	{
		if (arg && arg[0] != '\0')
		{
			ft_putstr_fd(arg, STDOUT_FILENO);
			if (has_next)
				ft_putstr_fd(" ", STDOUT_FILENO);
		}
	}
}

int	echo_builtin(t_data *data)
{
	int	i;
	int	print_new_line;

	print_new_line = 1;
	i = 1;
	if (!data->ast->args[1])
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (1);
	}
	while (data->ast->args[i] && check_n_option(data->ast->args[i]))
	{
		print_new_line = 0;
		i++;
	}
	while (data->ast->args[i])
	{
		print_arg(data, data->ast->args[i], data->ast->args[i + 1] != NULL);
		i++;
	}
	if (print_new_line)
		ft_putstr_fd("\n", STDOUT_FILENO);
	data->wstatus = 0;
	return (1);
}
