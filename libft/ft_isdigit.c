/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:06:46 by paromero          #+#    #+#             */
/*   Updated: 2023/10/10 11:25:46 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c < 48 || c > 57)
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

    tmp = ft_isdigit(c);
    if (tmp == 1){
    printf("es un número");
    }
    else
    printf("no es un número");
    return 0;
}
*/
