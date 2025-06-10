/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:14:24 by anggalle          #+#    #+#             */
/*   Updated: 2025/04/09 18:32:45 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_exit_numeric_error(t_data *data, char *arg_str)
{
	ft_putstr_fd("-minishell: exit: ", STDERR_FILENO);
	if (arg_str)
		ft_putstr_fd(arg_str, STDERR_FILENO);
	else
		ft_putstr_fd("", STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	data->wstatus = 2;
	free_minishell(data);
	exit(2);
}

static int	validate_numeric_exit_arg(t_data *data, char *arg_str)
{
	int	i;

	i = 0;
	if (!arg_str || arg_str[0] == '\0')
		return (handle_exit_numeric_error(data, arg_str));
	if (arg_str[0] == '+' || arg_str[0] == '-')
	{
		i++;
		if (arg_str[i] == '\0')
			return (handle_exit_numeric_error(data, arg_str));
	}
	while (arg_str[i])
	{
		if (!ft_isdigit(arg_str[i]))
			return (handle_exit_numeric_error(data, arg_str));
		i++;
	}
	return (0);
}

int	exit_builtin(t_data *data, t_ast *ast)
{
	int	exit_code;

	if (data->ast->args[1])
	{
		if (validate_numeric_exit_arg(data, data->ast->args[1]))
			return (1);
		if (ast->args[2])
		{
			ft_putstr_fd("-minishell: exit: too many arguments\n",
				STDERR_FILENO);
			data->wstatus = 1;
			return (1);
		}
		exit_code = ft_atoi(data->ast->args[1]);
		free_minishell(data);
		exit(exit_code);
	}
	else
	{
		data->exit = 1;
		data->wstatus = 0;
	}
	return (1);
}
