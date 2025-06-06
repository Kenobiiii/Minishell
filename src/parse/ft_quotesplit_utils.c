/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotesplit_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:40:03 by paromero          #+#    #+#             */
/*   Updated: 2025/05/27 19:26:06 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_double_operator(const char *s, size_t i)
{
	return ((s[i] == '<' && s[i + 1] == '<')
		|| (s[i] == '>' && s[i + 1] == '>')
		|| (s[i] == '2' && s[i + 1] == '>'));
}

void	handle_special_operators(const char *s, size_t *i)
{
	if (is_double_operator(s, *i))
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
