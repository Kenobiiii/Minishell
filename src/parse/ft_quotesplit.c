/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 09:38:49 by paromero          #+#    #+#             */
/*   Updated: 2025/01/28 13:13:06 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int count_substr(char const *s, char c)
{
	size_t i = 0;
	size_t count = 0;
	int quotecount = 0;

	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (!s[i])
			break;
		count++;
		if (s[i] == '"' || s[i] == '\'')
		{
			quotecount++;
			char quote = s[i++];
			while (s[i] && (s[i] != quote || quotecount % 2 != 0))
			{
				if (s[i] == quote)
					quotecount++;
				i++;
			}
			if (s[i] == quote)
				i++;
		}
		else
		{
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static int allocate_substr(char **array, char const *s, char c)
{
	size_t i;
	size_t j;
	size_t end;
	size_t start;
	char quote;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (!s[i])
			break ;
		start = i;
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i++];
			start = i;
			while (s[i] && s[i] != quote)
				i++;
			end = i;
			if (s[i] == quote)
				i++;
			array[j] = ft_substr(s, start, end - start);
		}
		else
		{
			while (s[i] && s[i] != c)
				i++;
			array[j] = ft_substr(s, start, i - start);
		}
		j++;
	}
	array[j] = NULL;
	return (0);
}

char	**ft_quotesplit(char const *s, char c)
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
		free_split(array);
		return (NULL);
	}
	return (array);
}
