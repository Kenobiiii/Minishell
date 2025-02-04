/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:32 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 11:45:37 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_handle_single(char	*line)
{
	char	*new_line;
	int		i;
	int		j;
	int		count;

	count = 0;
	j = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			i++;
		i++;
		count++;
	}
	new_line = (char *)malloc(sizeof(char) * (count + 1));
	if (!new_line)
		return (0);
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			i++;
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
	int		count;

	count = 0;
	j = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '"')
			i++;
		i++;
		count++;
	}
	new_line = (char *)malloc(sizeof(char) * (count + 1));
	if (!new_line)
		return (0);
	i = 0;
	while (line[i])
	{
		if (line[i] == '"')
			i++;
		new_line[j++] = line[i];
		i++;
	}
	new_line[j] = '\0';
	return (new_line);
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
		while(matrix[count_x][count_y])
		{
			if (matrix[count_x][count_y] == '\'')
			{
				new_line = ft_handle_single(matrix[count_x]);
				free (matrix[count_x]);
				matrix[count_x] = new_line;
				quote_found = 1;
				break ;
			}
			else if (matrix[count_x][count_y] == '"')
			{
				new_line = ft_handle_double(matrix[count_x]);
				new_line = ft_handledollar(data, new_line);
				free (matrix[count_x]);
				matrix[count_x] = new_line;
				quote_found = 1;
				break ;
			}
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
	//printf("%s\n", new_line);
}
