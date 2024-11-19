/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 11:10:30 by paromero          #+#    #+#             */
/*   Updated: 2023/10/10 11:10:54 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	size_t	s;

	s = n;
	if (n < 0)
	{
		s = s * (-1);
		ft_putchar_fd('-', fd);
	}
	if (s >= 10)
	{
		ft_putnbr_fd((s / 10), fd);
	}
	ft_putchar_fd(((s % 10) + '0'), fd);
}
