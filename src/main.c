/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:54:48 by paromero          #+#    #+#             */
/*   Updated: 2024/11/25 20:33:49 by paromero         ###   ########.fr       */
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
		//TODO Signal handler (Importante!!!!)
		read = readline(data.prompt);
		if (read == NULL)
			break;
		if (ft_strlen(read) == 0)
			continue;
		add_history(read);
		if (ft_strncmp(read, "exit", 5) == 0)
			data.exit = 1;
		if (read) //TODO && sintax_handler)
		{
			ft_tokens(read);
			//TODO func parseo
			//TODO func ejecutable
		}
	}
}