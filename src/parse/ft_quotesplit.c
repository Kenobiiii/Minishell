/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 09:38:49 by paromero          #+#    #+#             */
/*   Updated: 2025/04/08 18:15:16 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	skip_quotes(const char *s, size_t *i, char c, char *quote)
{
	while (s[*i] && (s[*i] != c || *quote))
	{
		if (s[*i] == '"' || s[*i] == '\'')
		{
			*quote = s[*i];
			(*i)++;
			while (s[*i] && s[*i] != *quote)
				(*i)++;
			if (s[*i] == *quote)
				(*i)++;
			*quote = '\0';
		}
		else if (s[*i] == '<' || s[*i] == '>' || s[*i] == '|')
			break ;
		else
			(*i)++;
	}
}

static int	count_substr(char const *s, char c)
{
	size_t	i;
	size_t	count;
	char	quote;

	i = 0;
	count = 0;
	quote = '\0';
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			break ;
		count++;
		if (s[i] == '<' || s[i] == '>' || s[i] == '|')
		{
			if ((s[i] == '<' && s[i + 1] == '<')
				|| (s[i] == '>' && s[i + 1] == '>'))
				i += 2;
			else
				i++;
		}
		else
			skip_quotes(s, &i, c, &quote);
	}
	return (count);
}

static int	handle_special_chars(char **array, char const *s, size_t *i)
{
	if ((s[*i] == '<' && s[*i + 1] == '<')
		|| (s[*i] == '>' && s[*i + 1] == '>'))
	{
		array[0] = ft_substr(s, *i, 2);
		(*i) += 2;
	}
	else
	{
		array[0] = ft_substr(s, *i, 1);
		(*i) += 1;
	}
	if (array[0] == NULL)
		return (-1);
	return (0);
}

static int	process_substr(char **array, char const *s, size_t *i, char c)
{
	size_t	start;
	char	quote;

	start = *i;
	quote = '\0';
	if (s[*i] == '<' || s[*i] == '>' || s[*i] == '|')
		return (handle_special_chars(array, s, i));
	while (s[*i] && (s[*i] != c || quote))
	{
		if (s[*i] == '"' || s[*i] == '\'')
		{
			quote = s[*i];
			(*i)++;
			while (s[*i] && s[*i] != quote)
				(*i)++;
			if (s[*i] == quote)
				(*i)++;
			quote = '\0';
		}
		else if (s[*i] == '<' || s[*i] == '>' || s[*i] == '|')
			break ;
		else
			(*i)++;
	}
	array[0] = ft_substr(s, start, *i - start);
	if (array[0] == NULL)
		return (-1);
	return (0);
}

static int	allocate_substr(char **array, char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			break ;
		if (process_substr(&array[j], s, &i, c) == -1)
			return (-1);
		j++;
	}
	array[j] = NULL;
	return (0);
}

char	**ft_quotesplit(char const *s, char c, t_data *data)
{
	char	**array;
	size_t	substrlen;

	if (!s)
		return (NULL);
	substrlen = count_substr(s, c);
	array = (char **)ft_calloc(substrlen + 1, sizeof(char *));
	if (!array)
		return (NULL);
	if (allocate_substr(array, s, c) == -1)
	{
		free_matrix(array);
		return (NULL);
	}
	ft_handle_quotes(data, array);
	return (array);
}
