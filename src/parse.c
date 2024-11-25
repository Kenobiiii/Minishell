/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 20:08:34 by paromero          #+#    #+#             */
/*   Updated: 2024/11/25 20:34:30 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * TODO arreglar segfault llega hasta 3 palabras y luego segfault
 * TODO func de tokens
 * TODO func de ast
 */

void	ft_tokens(char *str)
{
    char	**result;
    int		i;

	i = 0;
	result = ft_split(str, ' ');
	while (result != NULL)
	{
		printf("palabra %d: %s\n", i, result[i]);
		printf("%d\n", i);
		i++;
	}
}