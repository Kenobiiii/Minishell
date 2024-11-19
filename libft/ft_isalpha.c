/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:05:04 by paromero          #+#    #+#             */
/*   Updated: 2023/10/05 10:30:08 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if (c < 65 || (c > 90 && c < 97) || c > 122)
	{
		return (0);
	}
	return (1);
}
/*
int main()
{
    int c;
    c = 'A';
    int tmp;
    
    tmp = ft_isalpha(c);
    if (tmp == 1){
    printf("es una letra");
    }
    else
    printf("no es una letra");
    return 0;
}
*/
