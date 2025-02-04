/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:54:12 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 17:44:48 by paromero         ###   ########.fr       */
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
