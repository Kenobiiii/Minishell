/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:32 by paromero          #+#    #+#             */
/*   Updated: 2025/05/29 17:27:49 by paromero         ###   ########.fr       */
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
	
	// Find the closing quote
	while (matrix[count_x][end] && matrix[count_x][end] != quote_type)
		end++;
	
	if (!matrix[count_x][end])
		return (0); // No closing quote found
	
	// Extract just this quoted segment (including quotes)
	processed_part = ft_substr(matrix[count_x], start, end - start + 1);
	if (!processed_part)
		return (0);
	
	// Process this segment based on quote type
	if (quote_type == '\'')
	{
		// Single quotes: just remove the quotes, no variable expansion
		temp = ft_handle_type_quote(processed_part, '\'');
		new_line = ft_mask_operator(temp);
		// Don't free temp here - ft_mask_operator already freed it
	}
	else if (quote_type == '"')
	{
		// Double quotes: remove quotes AND expand variables
		temp = ft_handle_type_quote(processed_part, '"');
		new_line = ft_handledollar(data, temp);
		free(temp);
		new_line = ft_mask_operator(new_line);
		// Don't free the old new_line here - ft_mask_operator already freed it
	}
	else
	{
		free(processed_part);
		return (0);
	}
	
	// Now reconstruct the line with the processed segment
	temp = malloc(ft_strlen(matrix[count_x]) + ft_strlen(new_line) + 1);
	if (!temp)
	{
		free(new_line);
		free(processed_part);
		return (0);
	}
	
	// Copy everything before the quote
	i = 0;
	while (i < start)
	{
		temp[i] = matrix[count_x][i];
		i++;
	}
	
	// Copy the processed content
	ft_strlcpy(temp + i, new_line, ft_strlen(new_line) + 1);
	i += ft_strlen(new_line);
	
	// Copy everything after the closing quote
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
			count_y = 0; // Restart from beginning to process other quotes
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
