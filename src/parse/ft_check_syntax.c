/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:29:43 by paromero          #+#    #+#             */
/*   Updated: 2025/06/02 17:33:46 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_start_pipe(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '|')
		return (0);
	return (1);
}

int	check_syntax(char *line)
{
	int		i;
	int		in_quote;
	char	quote_type;

	i = 0;
	in_quote = 0;
	quote_type = 0;
	if (!check_start_pipe(line))
		return (0);
	while (line[i])
	{
		if (!check_syntax_loop(line, &i, &in_quote, &quote_type))
			return (0);
	}
	return (1);
}
