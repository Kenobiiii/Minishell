/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2024/11/20 17:48:40 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **env)
{
    t_data data;
    //char *read;
    
    if (ac != 1)
    {
        printf("Error: More than one argument");
        return (0);
    }
    (void)ac;
    (void)av;
    init_data(&data, env);    
}