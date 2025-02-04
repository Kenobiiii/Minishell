/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:32 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 13:15:42 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_quotelen(char	*line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			i++;
		if (line[i])
			i++;
		count++;
	}
	return (count);
}

char	*ft_handle_single(char	*line)
{
	char	*new_line;
	int		i;
	int		j;
	int		count;

	j = 0;
	i = 0;
	count = ft_quotelen(line);
	new_line = (char *)malloc(sizeof(char) * (count + 1));
	if (!new_line)
		return (0);
	while (line[i])
	{
		if (line[i] == '\'')
			i++;
		new_line[j++] = line[i];
		if (line[i])
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
	int		count;

	count = 0;
	j = 0;
	i = 0;
	count = ft_quotelen(line);
	new_line = (char *)malloc(sizeof(char) * (count + 1));
	if (!new_line)
		return (0);
	while (line[i])
	{
		if (line[i] == '"')
			i++;
		new_line[j++] = line[i];
		if (line[i])
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
		free(matrix[count_x]);
		matrix[count_x] = new_line;
		return (1);
	}
	else if (matrix[count_x][count_y] == '"')
	{
		new_line = ft_handle_double(matrix[count_x]);
		new_line = ft_handledollar(data, new_line);
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
