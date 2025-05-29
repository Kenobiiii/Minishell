/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/05/29 17:26:23 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_handle_type_quote(char *line, char quote_char)
{
	char	*new_line;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(line);
	if (len < 2)
		return (ft_strdup(line)); // No quotes to remove
	
	new_line = (char *)malloc(sizeof(char) * len);
	if (!new_line)
		return (NULL);
	
	j = 0;
	i = 0;
	
	// Skip the first quote if it matches
	if (line[i] == quote_char)
		i++;
	
	// Copy everything except the last quote
	while (i < len && !(i == len - 1 && line[i] == quote_char))
	{
		new_line[j++] = line[i];
		i++;
	}
	
	new_line[j] = '\0';
	return (new_line);
}

int	ft_quotelen(char	*line, char quote)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (line[i] == quote)
		i++;
	while (line[i] && line[i] != quote)
	{
		count++;
		i++;
	}
	return (count);
}
