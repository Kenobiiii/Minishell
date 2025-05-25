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

static int	g_in_execution = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	if (g_in_execution)
	{
		// Durante ejecución de comando: solo nueva línea
		write(STDOUT_FILENO, "\n", 1);
		g_sigint_received = 1;
	}
	else
	{
		// En el prompt: limpiar línea actual y mostrar nuevo prompt
		g_sigint_received = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigquit(int sig)
{
	(void)sig;
	if (g_in_execution)
	{
		// Durante ejecución: mostrar mensaje y continuar
		write(STDOUT_FILENO, "Quit: 3\n", 8);
	}
	// En el prompt: ignorar (no hacer nada)
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);   // Ctrl+C
	signal(SIGQUIT, handle_sigquit); // Ctrl+backslash
}

void	set_execution_mode(int mode)
{
	g_in_execution = mode;
}

void	setup_signals_for_child(void)
{
	signal(SIGINT, SIG_DFL);  // Restaurar comportamiento por defecto
	signal(SIGQUIT, SIG_DFL); // Restaurar comportamiento por defecto
}
