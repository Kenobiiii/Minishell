/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:04:06 by paromero          #+#    #+#             */
/*   Updated: 2023/10/09 11:39:54 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*s1;

	s1 = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (s1[i] == (unsigned char)c)
		{
			return ((void *)&s1[i]);
		}
		i++;
	}
	return (NULL);
}
/*int main ()
{
 const void *largestring = "buenos dias";
           int c = 'o';
           char *ptr;
           ptr = ft_memchr(largestring, c, 11);
   printf("%s\n", ptr);
   return(0);
}*/
