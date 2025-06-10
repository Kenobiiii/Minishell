/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:42:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/06/10 19:56:31 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	if (g_shell_state == STATE_PROMPT_NORMAL
		|| g_shell_state == STATE_PROMPT_INTERRUPTED)
	{
		g_shell_state = STATE_PROMPT_INTERRUPTED;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_shell_state == STATE_EXECUTING
		|| g_shell_state == STATE_EXECUTION_INTERRUPTED
		|| g_shell_state == STATE_EXECUTION_INTERRUPTED_SIGQUIT)
	{
		g_shell_state = STATE_EXECUTION_INTERRUPTED;
	}
}

void	handle_sigquit(int sig)
{
	(void)sig;
	if (g_shell_state == STATE_EXECUTING
		|| g_shell_state == STATE_EXECUTION_INTERRUPTED
		|| g_shell_state == STATE_EXECUTION_INTERRUPTED_SIGQUIT)
	{
		g_shell_state = STATE_EXECUTION_INTERRUPTED_SIGQUIT;
	}
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	configure_signals(int mode)
{
	if (mode == 0)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
	}
	else if (mode == 1)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (mode == 2)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
}
