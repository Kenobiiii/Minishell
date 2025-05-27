/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:29:43 by paromero          #+#    #+#             */
/*   Updated: 2025/05/27 19:26:06 by paromero         ###   ########.fr       */
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

static void	handle_syntax_quotes(char *line, int *i, int *in_quote,
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

static int	check_pipe_syntax(char *line, int *i)
{
	(*i)++;
	while (line[*i] == ' ')
		(*i)++;
	if (line[*i] == '|' || line[*i] == '\0')
		return (0);
	return (1);
}

static int	check_redir_syntax(char *line, int *i)
{
	char	redir;

	redir = line[(*i)++];
	if (line[*i] == redir)
		(*i)++;
	while (line[*i] == ' ')
		(*i)++;
	if (line[*i] == '\0' || line[*i] == '|'
		|| line[*i] == '<' || line[*i] == '>')
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
		handle_syntax_quotes(line, &i, &in_quote, &quote_type);
		if (!in_quote)
		{
			if (line[i] == '|' && !check_pipe_syntax(line, &i))
				return (0);
			else if ((line[i] == '<' || line[i] == '>')
				&& !check_redir_syntax(line, &i))
				return (0);
		}
		if (line[i] && !(line[i] == '|' && !in_quote))
			i++;
	}
	return (1);
}
