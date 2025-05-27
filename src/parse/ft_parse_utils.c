/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:47:59 by paromero          #+#    #+#             */
/*   Updated: 2025/05/27 19:26:06 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_dobletype(t_type	type)
{
	if (type == REDOUT2 || type == REDIN2
		|| type == AND || type == OR)
		return (1);
	return (0);
}

size_t	ft_spacestrlen(char *line)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (line[i] != '\0')
	{
		j++;
		if (ft_types(line + i) != CMD)
		{
			j += 2;
			if (line[i + 1] && line[i + 1] == line[i])
				i++;
		}
		i++;
	}
	return (j);
}

char	*check_access(char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			errno = EACCES;
	}
	return (NULL);
}

int	handle_special_chars(char **array, char const *s, size_t *i)
{
	if (is_double_operator(s, *i))
	{
		array[0] = ft_substr(s, *i, 2);
		(*i) += 2;
	}
	else
	{
		array[0] = ft_substr(s, *i, 1);
		(*i) += 1;
	}
	if (array[0] == NULL)
		return (-1);
	return (0);
}
