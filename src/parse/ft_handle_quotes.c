/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:32 by paromero          #+#    #+#             */
/*   Updated: 2025/05/30 12:12:01 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_detect_quote_type(t_data *data, char **matrix, int count_x, int count_y)
{
	char	*new_line;
	char	*temp;
	char	*processed_part;
	char	quote_type;
	int		start;
	int		end;
	int		i;

	quote_type = matrix[count_x][count_y];
	start = count_y;
	end = start + 1;
	while (matrix[count_x][end] && matrix[count_x][end] != quote_type)
		end++;
	if (!matrix[count_x][end])
		return (0);
	processed_part = ft_substr(matrix[count_x], start, end - start + 1);
	if (!processed_part)
		return (0);
	if (quote_type == '\'')
	{
		temp = ft_handle_type_quote(processed_part, '\'');
		new_line = ft_mask_operator(temp);
	}
	else if (quote_type == '"')
	{
		temp = ft_handle_type_quote(processed_part, '"');
		new_line = ft_handledollar(data, temp);
		free(temp);
		new_line = ft_mask_operator(new_line);
	}
	else
	{
		free(processed_part);
		return (0);
	}
	temp = malloc(ft_strlen(matrix[count_x]) + ft_strlen(new_line) + 1);
	if (!temp)
	{
		free(new_line);
		free(processed_part);
		return (0);
	}
	i = 0;
	while (i < start)
	{
		temp[i] = matrix[count_x][i];
		i++;
	}
	ft_strlcpy(temp + i, new_line, ft_strlen(new_line) + 1);
	i += ft_strlen(new_line);
	ft_strlcpy(temp + i, matrix[count_x] + end + 1, ft_strlen(matrix[count_x] + end + 1) + 1);
	free(matrix[count_x]);
	free(new_line);
	free(processed_part);
	matrix[count_x] = temp;
	return (1);
}

static int	ft_find_quotes_in_line(t_data *data, char **matrix, int count_x)
{
	int	count_y;
	int	quote_found;

	quote_found = 0;
	count_y = 0;
	while (matrix[count_x][count_y])
	{
		if (ft_detect_quote_type(data, matrix, count_x, count_y))
		{
			quote_found = 1;
			count_y = 0;
		}
		else
			count_y++;
	}
	return (quote_found);
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
			new_line = ft_handledollar(data, matrix[count_x]);
			free(matrix[count_x]);
			matrix[count_x] = new_line;
		}
		count_x++;
	}
}
