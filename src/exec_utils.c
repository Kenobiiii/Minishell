/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:59:23 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/21 14:04:05 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <string.h>

// Función para contar los nodos de la lista enlazada
static size_t count_nodes(t_env *head)
{
    size_t count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

// Función para convertir la lista enlazada a matriz
const char **list_to_array(t_env *head)
{
    size_t count = count_nodes(head);
    const char **array = malloc((count + 1) * sizeof(char *));
    if (!array)
        return NULL;

    size_t i = 0;
    while (head) {
        array[i] = strdup(head->value); // Copia el valor a la matriz
        if (!array[i]) {
            // Liberar memoria en caso de error
            for (size_t j = 0; j < i; j++) {
                free((char *)array[j]);
            }
            free(array);
            return NULL;
        }
        head = head->next;
        i++;
    }
    array[i] = NULL; // Terminar la matriz con NULL
    return (array);
}

// Liberar memoria de la matriz
void free_array(const char **array) {
    if (!array)
        return;
    for (size_t i = 0; array[i]; i++) {
        free((char *)array[i]);
    }
    free(array);
}
