/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 11:22:14 by paromero          #+#    #+#             */
/*   Updated: 2023/10/11 09:44:27 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *str1, const void *str2, size_t n)
{
	unsigned char		*p_str1;
	unsigned const char	*p_str2;

	if (str1 < str2)
	{
		return (ft_memcpy(str1, str2, n));
	}
	p_str1 = (unsigned char *)str1;
	p_str2 = (unsigned const char *)str2;
	if (!n || str1 == str2)
		return (str1);
	while (n--)
		p_str1[n] = p_str2[n];
	return (str1);
}
/*int		main()

    char src[] = "HOLAAA";
	char dst[] = "holaaaa";
	size_t len = 7;
	write(1, ft_memmove(dst, src, len), 8);
	write(1, "\n", 1);
	write(1, memmove(dst, src, len), 8);
    return 0;
}*/
