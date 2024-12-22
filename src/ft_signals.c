/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:08:47 by paromero          #+#    #+#             */
/*   Updated: 2024/12/11 17:43:57 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void signal_handler(int sig)
{
    if (sig == SIGINT) // Ctrl-C
    {
        printf("\n");
        rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
    }
    else if (sig == SIGQUIT) // Ctrl-
    {
        // Ignora esta señal en el padre
		write(1, "1", 1);
        write(1, "\b\b  \b\b", 6); // Borra el ^\ que aparece al pulsar
    }
}

void	configure_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}