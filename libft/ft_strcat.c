/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:20:31 by paromero          #+#    #+#             */
/*   Updated: 2024/11/20 18:28:05 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *src, char *dst)
{
	char	*result;
	int		j;
	int		k;

	j = 0;
	k = ft_strlen(src) + ft_strlen(dst) + 1;
	result = (char *)malloc(sizeof(char) * k);
	if (result == NULL)
		return (NULL);
	k = 0;
	while (src[j] != '\0')
	{
		result[k] = src[j];
		j++;
		k++;
	}
	j = 0;
	while (dst[j] != '\0')
	{
		result[k] = dst[j];
		j++;
		k++;
	}
	result[k] = '\0';
	return (result);
}
