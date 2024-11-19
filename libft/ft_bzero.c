/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 09:58:44 by paromero          #+#    #+#             */
/*   Updated: 2023/10/17 12:39:40 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*b;

	i = 0;
	b = (char *)s;
	while (i < n)
	{
		b[i] = 0;
		i++;
	}
	return (s);
}
/*int		main()
{
	char s[] = "holaaaa";
	size_t len = 5;
	write(1, ft_bzero(s, len), 8);
    return 0;
}*/
