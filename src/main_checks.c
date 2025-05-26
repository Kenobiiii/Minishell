/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:00:00 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/26 10:20:00 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_syntax_errors(t_data *data)
{
	if (!check_syntax(data->line))
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		data->wstatus = 2;
		return (free_while(data));
	}
	if (!openquotes(data->line))
	{
		perror("command not found");
		data->wstatus = 127;
		return (free_while(data));
	}
	return (1);
}

int	check_line_errors(t_data *data)
{
	if (!ft_isspace(data->line))
		return (free_while(data));
	if (!check_syntax_errors(data))
		return (0);
	if (!handle_invslash_pcomma(data->line))
		return (free_while(data));
	return (1);
}
