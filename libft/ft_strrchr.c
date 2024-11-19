/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:52:09 by paromero          #+#    #+#             */
/*   Updated: 2023/10/25 12:15:56 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*s1;
	int		tmp;

	s1 = (char *)s;
	tmp = ft_strlen(s1);
	while (tmp >= 0)
	{
		if (s1[tmp] == (unsigned char)c)
		{
			return (&s1[tmp]);
		}
		tmp--;
	}
	return (NULL);
}
/*int main ()
{
 const char *largestring = "buenos dias";
           int c = 'a';
    printf("%s\n", ft_strrchr(largestring, c));
   printf("%s\n", strrchr(largestring, c));
   return(0);
}*/
