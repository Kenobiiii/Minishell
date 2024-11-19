/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:21:22 by paromero          #+#    #+#             */
/*   Updated: 2023/10/26 13:48:06 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	negative(int n)
{
	if (n < 0)
		return (n * -1);
	else
		return (n);
}

char	*ft_itoa(int n)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = n;
	if (j <= 0)
		i++;
	while (j)
	{
		j /= 10;
		i++;
	}
	tmp = malloc(i + 1);
	if (!tmp)
		return (NULL);
	tmp[i] = '\0';
	while (i--)
	{
		tmp[i] = (negative(n % 10) + '0');
		n /= 10;
	}
	if (tmp[0] == '0' && tmp[1] != '\0')
		tmp[0] = '-';
	return (tmp);
}
