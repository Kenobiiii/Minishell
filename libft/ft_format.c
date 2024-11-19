/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 10:40:37 by paromero          #+#    #+#             */
/*   Updated: 2024/04/16 10:15:33 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(char c)
{
	write (1, &c, 1);
	return (1);
}

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (*str)
	{
		i += ft_putchar(*str);
		str++;
	}
	return (i);
}

int	ft_putnbr(long long n)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		i += ft_putchar('-');
		i += ft_putnbr(-n);
	}
	else
	{
		if (n > 9)
			i += ft_putnbr(n / 10);
		i += ft_putchar((n % 10) + '0');
	}
	return (i);
}

int	ft_puthex(uintptr_t n, int is_uppercase, int include_prefix)
{
	int		i;
	char	hex_digit;

	i = 0;
	if (n == 0)
	{
		if (include_prefix)
			i += ft_putstr("0x");
		return (i + ft_putchar('0'));
	}
	if (include_prefix)
		i += ft_putstr("0x");
	if (n >= 16)
		i += ft_puthex(n / 16, is_uppercase, 0);
	hex_digit = n % 16;
	if (hex_digit > 9)
	{
		if (is_uppercase)
			i += ft_putchar('A' + hex_digit - 10);
		else
			i += ft_putchar('a' + hex_digit - 10);
	}
	else
		i += ft_putchar('0' + hex_digit);
	return (i);
}
