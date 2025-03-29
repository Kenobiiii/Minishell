/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 09:38:49 by paromero          #+#    #+#             */
/*   Updated: 2025/03/29 17:59:41 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int count_substr(char const *s, char c)
{
    size_t i = 0;
    size_t count = 0;
    char quote = '\0';

    while (s[i])
    {
        while (s[i] == c)
            i++;
        if (!s[i])
            break;
        count++;
        while (s[i] && (s[i] != c || quote))
        {
            if (s[i] == '"' || s[i] == '\'')
            {
                quote = s[i++];
                while (s[i] && s[i] != quote)
                    i++;
                if (s[i] == quote)
                    i++;
                quote = '\0';
            }
            else if ((s[i] == '<' && s[i + 1] == '<') || (s[i] == '>' && s[i + 1] == '>'))
            {
                // Detecta redirecciones dobles (<< o >>)
                i += 2; // Avanza dos posiciones
                break;
            }
            else if (s[i] == '<' || s[i] == '>' || s[i] == '|')
            {
                // Detecta redirecciones simples (<, >, |)
                i++;
                break;
            }
            else
                i++;
        }
    }
    return (count);
}


static int process_substr(char **array, char const *s, size_t *i, char c)
{
	size_t start = *i;
	char quote = '\0';

	// Si encuentra un carácter especial al inicio, lo trata como un token independiente
	if (s[*i] == '<' || s[*i] == '>' || s[*i] == '|')
	{
		if ((s[*i] == '<' && s[*i + 1] == '<') || (s[*i] == '>' && s[*i + 1] == '>'))
		{
			// Detecta redirecciones dobles (<< o >>)
			array[0] = ft_substr(s, *i, 2); // Extrae los dos caracteres
			(*i) += 2;
		}
		else
		{
			// Detecta redirecciones simples (<, >, |)
			array[0] = ft_substr(s, *i, 1); // Extrae solo el carácter especial
			(*i)++;
		}
		return (array[0] ? 0 : -1);
	}

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
		{
			// Si encuentra un carácter especial en medio, corta el token actual
			break;
		}
		else
			(*i)++;
	}

	array[0] = ft_substr(s, start, *i - start);
	return (array[0] ? 0 : -1);
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
