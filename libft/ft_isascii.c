/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:21:37 by paromero          #+#    #+#             */
/*   Updated: 2023/10/05 10:13:31 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
	{
		return (1);
	}
	else
		return (0);
}
/*
int main()
{
    int c;
    c = 'A';
    int tmp;
    
    tmp = ft_isascii(c);
    if (tmp == 1){
    printf("es una ascii");
    }
    else
    printf("no es una ascii");
    return 0;
}

*/
