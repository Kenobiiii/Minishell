/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:32 by paromero          #+#    #+#             */
/*   Updated: 2025/03/26 18:45:55 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*ft_handle_single(char	*line)
{
	char	*new_line;
	int		i;
	int		j;

	j = 0;
	i = 0;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!new_line)
		return (0);
	while (line[i])
	{
		if (line[i] != '\'')
			new_line[j++] = line[i];
		i++;
	}
	new_line[j] = '\0';
	return (new_line);
}

char	*ft_handle_double(char	*line)
{
	char	*new_line;
	int		i;
	int		j;

	j = 0;
	i = 0;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!new_line)
		return (0);
	while (line[i])
	{
		if (line[i] != '"')
			new_line[j++] = line[i];
		i++;
	}
	new_line[j] = '\0';
	return (new_line);
}

int	handle_quotes(t_data *data, char **matrix, int count_x, int count_y)
{
	char	*new_line;

	if (matrix[count_x][count_y] == '\'')
	{
		new_line = ft_handle_single(matrix[count_x]);
		new_line = ft_mask_operator(new_line);
		free(matrix[count_x]);
		matrix[count_x] = new_line;
		return (1);
	}
	else if (matrix[count_x][count_y] == '"')
	{
		new_line = ft_handle_double(matrix[count_x]);
		new_line = ft_handledollar(data, new_line);
		new_line = ft_mask_operator(new_line);
		free(matrix[count_x]);
		matrix[count_x] = new_line;
		return (1);
	}
	return (0);
}

void	ft_handle_quotes(t_data	*data, char	**matrix)
{
	char	*new_line;
	int		count_x;
	int		count_y;
	int		quote_found;

	count_x = 0;
	new_line = NULL;
	while (matrix[count_x])
	{
		quote_found = 0;
		count_y = 0;
		while (matrix[count_x][count_y] && quote_found != 1)
		{
			quote_found = handle_quotes(data, matrix, count_x, count_y);
			count_y++;
		}
		if (!quote_found)
		{
			new_line = ft_handledollar(data, matrix[count_x]);
			free(matrix[count_x]);
			matrix[count_x] = new_line;
		}
		count_x++;
	}
}
