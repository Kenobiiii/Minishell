/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:47:59 by paromero          #+#    #+#             */
/*   Updated: 2025/06/02 17:33:46 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
