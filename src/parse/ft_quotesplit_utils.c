/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotesplit_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:40:03 by paromero          #+#    #+#             */
/*   Updated: 2025/04/08 19:40:56 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_special_operators(const char *s, size_t *i)
{
	if ((s[*i] == '<' && s[*i + 1] == '<')
		|| (s[*i] == '>' && s[*i + 1] == '>'))
		*i += 2;
	else
		(*i)++;
}

void	process_quotes(const char *s, size_t *i, char *quote)
{
	*quote = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != *quote)
		(*i)++;
	if (s[*i] == *quote)
		(*i)++;
	*quote = '\0';
}
