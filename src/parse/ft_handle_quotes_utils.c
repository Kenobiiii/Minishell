/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/05 18:11:24 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*remove_quote_delimiters(char *line, char quote_char)
{
	char	*new_line;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(line);
	if (len < 2)
		return (ft_strdup(line));
	new_line = (char *)malloc(sizeof(char) * len);
	if (!new_line)
		return (NULL);
	j = 0;
	i = 0;
	if (line[i] == quote_char)
		i++;
	while (i < len && !(i == len - 1 && line[i] == quote_char))
	{
		new_line[j++] = line[i];
		i++;
	}
	new_line[j] = '\0';
	return (new_line);
}

int	calculate_quote_content_length(char	*line, char quote)
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

void	ft_process_all_quotes(t_data	*data, char	**matrix)
{
	char	*new_line;
	int		count_x;
	int		quote_found;

	count_x = 0;
	while (matrix[count_x])
	{
		quote_found = ft_find_quotes_in_line(data, matrix, count_x);
		if (!quote_found)
		{
			new_line = expand_dollar_variables(data, matrix[count_x]);
			free(matrix[count_x]);
			matrix[count_x] = new_line;
		}
		count_x++;
	}
}
