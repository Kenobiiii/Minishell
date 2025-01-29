/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:42:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/01/29 20:21:07 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_sigint(int sig) {
	(void)sig;
	g_sigint_received = 1;
	write(STDOUT_FILENO, "\n", 1); // Nueva línea
	rl_replace_line("", 0);        // Limpiar entrada actual (requiere -lreadline)
	rl_on_new_line();              // Prepara nuevo prompt
	//rl_redisplay();            // Fuerza a readline a mostrar el prompt
}

// void handle_sigint(int sig)
// {
// 	(void)sig;
// 	ft_putstr_fd("\n", STDOUT_FILENO);
// 	minishell(list_to_array(data.env));
// }

void setup_signals(void)
{
	signal(SIGINT, handle_sigint);    // Ctrl+C
	signal(SIGQUIT, SIG_IGN);         // Ctrl+\ (ignorar)
}
	