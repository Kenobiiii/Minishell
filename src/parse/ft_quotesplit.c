/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 09:38:49 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 13:40:02 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	count_substr(char const	*s, char c)
{
	size_t	i;
	size_t	count;
	char	quote;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (!s[i])
			break ;
		count++;
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i++];
			while (s[i] && (s[i] != quote))
				i++;
			i++;
		}
		else
			while (s[i] && s[i] != c)
				i++;
	}
	return (count);
}

static int	process_substr(char **array, char const *s, size_t *i, char c)
{
	size_t	start;
	char	quote;

	start = *i;
	quote = '\0';
	if (s[*i] == '\'' || s[*i] == '"')
	{
		quote = s[*i];
		start = (*i)++;
		while (s[*i] && s[*i] != quote)
			(*i)++;
		if (s[*i] == quote)
			(*i)++;
	}
	else
	{
		while (s[*i] && s[*i] != c)
			(*i)++;
	}
	array[0] = ft_substr(s, start, *i - start);
	if (!array[0])
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

char	**ft_quotesplit(char const *s, char c, t_data	*data)
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
