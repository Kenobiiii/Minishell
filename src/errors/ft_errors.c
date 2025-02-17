/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:54:12 by paromero          #+#    #+#             */
/*   Updated: 2025/02/17 19:11:18 by anggalle         ###   ########.fr       */
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
		printf("Error: %s\n", error_message);
	free_minishell(data);
	exit(exit_code);
}
