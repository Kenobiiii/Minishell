/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:44:59 by paromero          #+#    #+#             */
/*   Updated: 2023/10/16 11:28:02 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c < 32 || c > 126)
	{
		return (0);
	}
	else
		return (1);
}
/*
int main()
{
    int c;
    c = 'A';
    int tmp;
    
    tmp = ft_isprint(c);
    if (tmp == 1){
    printf("es un caracter printable");
    }
    else
    printf("no es caracter printable");
    return 0;
}
*/
