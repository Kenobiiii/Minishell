/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:03:10 by paromero          #+#    #+#             */
/*   Updated: 2023/10/24 12:00:33 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*result;
	char	*s;
	size_t	i;

	i = 0;
	s = (char *)s1;
	result = (char *)malloc(ft_strlen(s1) + 1);
	if (result == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
/*int main() {
    const char *original = "Buenos dias!";
    char *copia = ft_strdup(original);
    
    if (copia != NULL) {
        printf("Copia: %s\n", copia);
        free(copia);
    } else {
        printf("Error al duplicar la cadena.\n");
    }
    
    return 0;
}*/
