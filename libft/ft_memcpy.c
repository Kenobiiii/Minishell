/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:42:50 by paromero          #+#    #+#             */
/*   Updated: 2023/10/24 11:01:08 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*s;
	unsigned char	*p;

	i = 0;
	s = dst;
	p = (unsigned char *)src;
	if (!src && !dst)
		return (NULL);
	while (i < n)
	{
		s[i] = p[i];
		i++;
	}
	return (dst);
}
/*int main ()
{
	size_t dstsize = 4;
	char dst[20];
    char src[] = "buenos dias";
    char tmp[20];
    
    memcpy(dst, src, dstsize);
    ft_memcpy(tmp, src, dstsize);
	printf("%s\n", dst);
	printf("%s\n", tmp);
}*/
