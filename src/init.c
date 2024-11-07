/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:53:02 by paromero          #+#    #+#             */
/*   Updated: 2024/11/07 19:15:41 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	init_data(t_data *data, char **env)
{
	data->line = NULL;
	data->last_exit_status = 0;
	data->signal_received = 0;
	data->pid = -1;
	data->prompt = "$Minishell> ";
	data->cwd = getcwd;
	data->tokens = NULL;
	data->ast = NULL;
}
