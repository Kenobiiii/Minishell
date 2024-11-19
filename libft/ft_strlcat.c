/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 10:14:08 by paromero          #+#    #+#             */
/*   Updated: 2023/10/20 11:26:20 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	lensrc;
	size_t	lendst;

	lensrc = ft_strlen(src);
	lendst = ft_strlen(dst);
	i = 0;
	if (dstsize <= lendst)
		return (dstsize + lensrc);
	while (src[i] != '\0' && (lendst + i) < (dstsize - 1))
	{
		dst[lendst + i] = src[i];
		i++;
	}
	dst[lendst + i] = '\0';
	return (lendst + lensrc);
}
/*int main ()

{
	size_t dstsize = 0;
	char dst[] = "aaa";
	const char src[] = "ASasdfadfasfd";
	printf("%zu\n", ft_strlcat(dst, src, dstsize));
}*/
