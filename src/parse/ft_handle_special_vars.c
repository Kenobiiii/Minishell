/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_special_vars.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:30:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/11 14:39:16 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_exit_status(t_data *data, char *result, int *j)
{
	char	*wstatus;
	int		k;

	wstatus = ft_itoa(data->wstatus);
	if (!wstatus)
		return ;
	k = 0;
	while (wstatus[k])
	{
		result[(*j)] = wstatus[k];
		(*j)++;
		k++;
	}
	free(wstatus);
}

void	handle_last_token(t_data *data, char *result, int *j)
{
	char	*last_token;
	int		k;

	if (!data || !result || !j)
		return ;
	last_token = data->last_token;
	if (!last_token)
		return ;
	k = 0;
	while (last_token[k])
	{
		result[(*j)] = last_token[k];
		(*j)++;
		k++;
	}
}
