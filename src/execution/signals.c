/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:42:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/04/09 17:40:15 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	write(STDOUT_FILENO, "\n", 1); // Nueva línea
	rl_replace_line("", 0); // Limpiar entrada actual (requiere -lreadline)
	rl_on_new_line(); // Prepara nuevo prompt
	//rl_redisplay(); // Fuerza a readline a mostrar el prompt
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint); // Ctrl+C
	signal(SIGQUIT, SIG_IGN); // Ctrl+\ (ignorar)
}
