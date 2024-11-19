/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 09:38:49 by paromero          #+#    #+#             */
/*   Updated: 2023/10/31 12:29:26 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_substr(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] != c && s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

static int	allocate_substr(char **array, char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	start;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		start = i;
		while (s[i] != c && s[i] != '\0')
			i++;
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

char	**ft_split(char const *s, char c)
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
/*int main()
{
    char str[] = "buenos días";
    char **result = ft_split(str, ' ');

    if (result)
    {
        for (int i = 0; result[i] != NULL; i++)
        {
            printf("palabra %d: %s\n", i, result[i]);
        }
    }
    return 0;
}*/
