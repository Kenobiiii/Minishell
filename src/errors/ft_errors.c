/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:54:12 by paromero          #+#    #+#             */
/*   Updated: 2025/05/14 18:41:56 by paromero         ###   ########.fr       */
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

void exit_minishell(t_data *data, const char *error_message, int exit_code)
{
    if (error_message)
    {
        if (errno == EACCES && exit_code == 127)
        {
            ft_putstr_fd("minishell: Permission denied\n", 2);
            exit_code = 126;  // Correct the exit code
        }
        else if (errno == ENOENT && exit_code == 127)
        {
            // Para comandos no encontrados, mostrar "command not found"
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(": command not found\n", 2);
        }
        else
            perror(error_message);
    }
    free_minishell(data);
    exit(exit_code);
}

/*int	handle_process_error(int value, const char *msg)
{
	if (value < 0)
	{
		perror(msg);
		return (-1);
	}
	return (value);
}*/
