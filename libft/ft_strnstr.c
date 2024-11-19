/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:29:46 by paromero          #+#    #+#             */
/*   Updated: 2023/10/09 09:59:17 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	tmp;

	i = 0;
	if (*needle == '\0' || needle == NULL)
	{
		return ((char *)haystack);
	}
	while (haystack[i] != '\0' && i < len)
	{
		tmp = 0;
		while (needle[tmp] == haystack[i + tmp] && i + tmp < len)
		{
			if (needle[tmp + 1] == '\0')
			{
				return ((char *)haystack + i);
			}
			tmp++;
		}
		i++;
	}
	return (NULL);
}

/*int main ()
{
 const char *str1 = "buenos dias";
           const char *str2 = "";
           char *ptr;
           ptr = ft_strnstr(str1, str2, 4);
   printf("%s\n", ptr);
   return(0);
}*/
