/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:32 by paromero          #+#    #+#             */
/*   Updated: 2025/06/04 13:52:48 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_quote_content(t_data *data, char *processed_part,
			char quote_type)
{
	char	*temp;
	char	*new_line;

	if (quote_type == '\'')
	{
		temp = ft_handle_type_quote(processed_part, '\'');
		new_line = temp;
	}
	else if (quote_type == '"')
	{
		temp = ft_handle_type_quote(processed_part, '"');
		new_line = ft_handledollar(data, temp);
		free(temp);
	}
	else
		return (NULL);
	return (new_line);
}

static char	*build_new_string(char *original, char *new_content,
			int start, int end)
{
	char	*result;
	int		i;

	result = malloc(ft_strlen(original) + ft_strlen(new_content) + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < start)
	{
		result[i] = original[i];
		i++;
	}
	ft_strlcpy(result + i, new_content, ft_strlen(new_content) + 1);
	i += ft_strlen(new_content);
	ft_strlcpy(result + i, original + end + 1,
		ft_strlen(original + end + 1) + 1);
	return (result);
}

static char	*extract_quote_section(char *line, int start,
	int *end, char quote_type)
{
	*end = start + 1;
	while (line[*end] && line[*end] != quote_type)
		(*end)++;
	if (!line[*end])
		return (NULL);
	return (ft_substr(line, start, *end - start + 1));
}

int	ft_detect_quote_type(t_data *data, char **matrix, int count_x, int count_y)
{
	char	*new_line;
	char	*processed_part;
	char	*temp;
	char	quote_type;
	int		end;

	quote_type = matrix[count_x][count_y];
	processed_part = extract_quote_section(matrix[count_x], count_y, &end,
			quote_type);
	if (!processed_part)
		return (-1);
	new_line = process_quote_content(data, processed_part, quote_type);
	if (!new_line)
	{
		free(processed_part);
		return (-1);
	}
	temp = build_new_string(matrix[count_x], new_line, count_y, end);
	free(matrix[count_x]);
	matrix[count_x] = temp;
	end = count_y + ft_strlen(new_line);
	free(new_line);
	free(processed_part);
	return (end);
}

int	ft_find_quotes_in_line(t_data *data, char **matrix, int count_x)
{
	int	count_y;
	int	quote_found;
	int	new_position;

	quote_found = 0;
	count_y = 0;
	while (matrix[count_x][count_y])
	{
		new_position = ft_detect_quote_type(data, matrix, count_x, count_y);
		if (new_position != -1)
		{
			quote_found = 1;
			count_y = new_position;
		}
		else
			count_y++;
	}
	return (quote_found);
}
