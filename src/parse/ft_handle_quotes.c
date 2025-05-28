/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:32 by paromero          #+#    #+#             */
/*   Updated: 2025/05/28 16:00:32 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_handle_single(char	*line)
{
	return (ft_handle_quote(line, '\''));
}

char	*ft_handle_double(char	*line)
{
	return (ft_handle_quote(line, '"'));
}

int	handle_quotes(t_data *data, char **matrix, int count_x, int count_y)
{
	char	*new_line;
	char	*temp;

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
		temp = ft_handledollar(data, new_line);
		free(new_line);
		new_line = ft_mask_operator(temp);
		free(matrix[count_x]);
		matrix[count_x] = new_line;
		return (1);
	}
	return (0);
}

static int	process_quotes_in_line(t_data *data, char **matrix, int count_x)
{
	int	count_y;
	int	quote_found;

	quote_found = 0;
	count_y = 0;
	while (quote_found != 1 && matrix[count_x][count_y])
	{
		if (handle_quotes(data, matrix, count_x, count_y))
			quote_found = 1;
		else
			count_y++;
	}
	return (quote_found);
}

void	ft_handle_quotes(t_data	*data, char	**matrix)
{
	char	*new_line;
	int		count_x;
	int		quote_found;

	count_x = 0;
	while (matrix[count_x])
	{
		quote_found = process_quotes_in_line(data, matrix, count_x);
		if (!quote_found)
		{
			new_line = ft_handledollar(data, matrix[count_x]);
			free(matrix[count_x]);
			matrix[count_x] = new_line;
		}
		count_x++;
	}
}
