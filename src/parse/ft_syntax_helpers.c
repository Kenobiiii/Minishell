/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/03 00:00:00 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_syntax_quotes(char *line, int *i, int *in_quote,
	char *quote_type)
{
	if ((line[*i] == '\'' || line[*i] == '"') && !*in_quote)
	{
		*in_quote = 1;
		*quote_type = line[*i];
	}
	else if (*in_quote && line[*i] == *quote_type)
		*in_quote = 0;
}

static int	skip_spaces_and_check(char *line, int *i, char check_char)
{
	while (line[*i] == ' ')
		(*i)++;
	if (check_char == '|' && (line[*i] == '|' || line[*i] == '\0'))
		return (0);
	if (check_char != '|' && (line[*i] == '\0' || line[*i] == '|'
			|| line[*i] == '<' || line[*i] == '>'))
		return (0);
	return (1);
}

int	check_operator_syntax(char *line, int *i, char operator)
{
	char	saved_char;

	if (operator == '|')
	{
		(*i)++;
		return (skip_spaces_and_check(line, i, '|'));
	}
	else if (operator == '<' || operator == '>')
	{
		saved_char = line[(*i)++];
		if (line[*i] == saved_char)
			(*i)++;
		return (skip_spaces_and_check(line, i, saved_char));
	}
	return (1);
}

int	check_logical_operators(char *line, int i)
{
	if ((line[i] == '&' && line[i + 1] == '&')
		|| (line[i] == '|' && line[i + 1] == '|'))
		return (0);
	return (1);
}

int	check_syntax_loop(char *line, int *i, int *in_quote, char *quote_type)
{
	handle_syntax_quotes(line, i, in_quote, quote_type);
	if (!*in_quote)
	{
		if (!check_logical_operators(line, *i))
			return (0);
		if (line[*i] == '|' && line[*i + 1] != '|'
			&& !check_operator_syntax(line, i, '|'))
			return (0);
		else if ((line[*i] == '<' || line[*i] == '>')
			&& !check_operator_syntax(line, i, line[*i]))
			return (0);
	}
	if (line[*i] && !(line[*i] == '|' && line[*i + 1] != '|' && !*in_quote))
		(*i)++;
	return (1);
}
