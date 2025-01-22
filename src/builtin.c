/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:17:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/22 13:33:11 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtins(t_data *data)
{
	int ret;

	ret = 0;
	if (ft_strncmp(data->ast->value, "echo", ft_strlen(data->ast->value)) == 0)
	{
		ret = echo_builtin(data);
	}
	return (ret);
}
