/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:54:12 by paromero          #+#    #+#             */
/*   Updated: 2025/04/09 17:52:16 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	syntax_error(void)
{
	printf("Syntax error\n");
	return (0);
}

char	*handle_invalid_cmd_path(char **cmd_path, char *total_cmd)
{
	free_cmd_path(cmd_path, total_cmd);
	return (NULL);
}
void	exit_minishell(t_data *data, const char *error_message, int exit_code)
{
	if (error_message)
    	perror(error_message);
	free_minishell(data);
	exit(exit_code);
}

int	handle_process_error(int value, const char *msg)
{
	if (value < 0)
	{
		perror(msg);
		return (-1);
	}
	return (value);
}
