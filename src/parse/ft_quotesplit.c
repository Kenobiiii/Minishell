/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 09:38:49 by paromero          #+#    #+#             */
/*   Updated: 2025/01/23 11:36:29 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_substr(char const *s, char c)
{
	size_t	i;
	size_t	count;
	int		quotecount;

	quotecount = 0;
	i = 0;
	count = 0;

	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] == '"' || s[i] == '\'')
		{
			quotecount++;
			i++;
		}
		if (s[i] != c && s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0' && quotecount % 2 != 0)
			i++;
		if (quotecount % 2 == 0)
		{
			i++;
			quotecount = 0;
		}
	}
	return (count);
}

static int	allocate_substr(char **array, char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	start;
	char	quote;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		start = i;
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i];
			i++;
			start = i;
			while (s[i] != quote && s[i] != '\0')
				i++;
		}
		else
		{	
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		if (i > start)
		{
			array[j] = ft_substr(s, start, i - start);
			if (array[j] == NULL)
				return (-1);
			j++;
		}
	}
	array[j] = NULL;
	return (0);
}

static void	free_mem(char **array)
{
	size_t	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
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
		free_mem(array);
		return (NULL);
	}
	return (array);
}

