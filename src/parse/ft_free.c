/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:02:07 by paromero          #+#    #+#             */
/*   Updated: 2025/01/29 18:20:27 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_innerwhile(t_data  *data)
{
	free (data->line);
	ft_free_tokens(data->tokens);
	data->tokens = NULL;
	ft_free_ast(data->ast);
	data->ast = NULL;
}