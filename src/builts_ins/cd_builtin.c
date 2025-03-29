/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:32:49 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/29 01:16:51 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd_builtin(t_data *data)
{
	const char	*home_dir;

	if (!data->ast->args[1])
	{
		home_dir = getenv("HOME");
		if (home_dir == NULL)
		{
			printf("HOME environment variable not set.\n");
			return (1);
		}
		if (chdir(home_dir) == -1)
		{
			printf("Can't get to the home directory.\n");
			return (1);
		}
	}
	else if (data->ast->args[2])
	{
		data->wstatus = 1;
	}
	else
	{
		if (chdir(data->ast->args[1]) == -1)
		{
			data->wstatus = 1;
			return (1);
		}
	}
	return (1);
}
