/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 10:08:08 by paromero          #+#    #+#             */
/*   Updated: 2024/04/26 10:20:51 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char *set)
{
	size_t	start;
	size_t	end;
	char	*str;

	str = 0;
	if (!s1 && !set)
		return (NULL);
	if (s1 != 0 && set != 0)
	{
		start = 0;
		end = ft_strlen(s1);
		while (s1[start] && ft_strchr(set, s1[start]))
			start++;
		while (end > start && s1[end - 1] && ft_strchr(set, s1[end - 1]))
			end--;
		str = (char *)malloc(end - start + 1);
		if (str)
			ft_strlcpy(str, &s1[start], end - start + 1);
	}
	return (str);
}
