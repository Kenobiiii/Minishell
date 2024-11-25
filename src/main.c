/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2024/11/25 18:01:20 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **env)
{
    t_data data;
    char *read;

    if (ac != 1)
    {
        printf("Error: More than one argument");
        return (0);
    }
    (void)ac;
    (void)av;
    init_data(&data, env);    
    while (data.exit == 0)
	{
		read = readline(data.prompt);
		if (read == NULL)
			break;
		if (ft_strlen(read) == 0)
			continue;
		add_history(read);
	}
}