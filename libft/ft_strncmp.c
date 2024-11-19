/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 10:09:57 by paromero          #+#    #+#             */
/*   Updated: 2023/10/18 10:27:11 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*s11;
	unsigned char	*s22;

	s11 = (unsigned char *)s1;
	s22 = (unsigned char *)s2;
	i = 0;
	while (i < n && (s11[i] != '\0' || s22[i] != '\0'))
	{
		if (s11[i] < s22[i] || s11[i] > s22[i])
		{
			return (s11[i] - s22[i]);
		}
		if (s11[i] == s22[i] && i == n)
		{
			return (0);
		}
		i++;
	}
	return (0);
}

/*int main ()
{
   char str1[15] = "ABCDEf";
   char str2[15] = "ABCDEF";
   int ret;

   ret = ft_strncmp(str1, str2, 6);
   printf ("%d\n", ret);
   printf ("%d\n", strncmp(str1, str2, 6));
   if(ret < 0)
   {
      printf("str1 menor str2");
   }
   else if(ret > 0)
   {
      printf("str2 menor str1");
   }
   else
   {
      printf("str1 igual str2");
   }

   return(0);
}*/
