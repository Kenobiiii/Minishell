/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 10:35:24 by paromero          #+#    #+#             */
/*   Updated: 2024/04/16 10:14:53 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_format(char format, va_list arg)
{
	int	i;

	i = 0;
	if (format == 'c')
		i += ft_putchar(va_arg(arg, int));
	else if (format == 's')
		i += ft_putstr(va_arg(arg, char *));
	else if (format == 'p')
		i += ft_puthex((uintptr_t)va_arg(arg, void *), 0, 1);
	else if (format == 'd')
		i += ft_putnbr(va_arg(arg, int));
	else if (format == 'i')
		i += ft_putnbr(va_arg(arg, int));
	else if (format == 'u')
		i += ft_putnbr((unsigned int)va_arg(arg, unsigned int));
	else if (format == 'x')
		i += ft_puthex((unsigned int)va_arg(arg, int), 0, 0);
	else if (format == 'X')
		i += ft_puthex((unsigned int)va_arg(arg, int), 1, 0);
	else if (format == '%')
		i += ft_putchar('%');
	return (i);
}

int	ft_printf(char const *format, ...)
{
	va_list	arg;
	int		i;
	int		final;

	va_start (arg, format);
	i = 0;
	final = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			i++;
			final += ft_format(format[i], arg);
		}
		else
			final += ft_putchar(format[i]);
		i++;
	}
	va_end(arg);
	return (final);
}
