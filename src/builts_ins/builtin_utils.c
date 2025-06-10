/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:17:47 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 15:52:22 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putstatus_fd(char *status, int fd)
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
