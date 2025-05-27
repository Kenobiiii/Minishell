/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:14:52 by paromero          #+#    #+#             */
/*   Updated: 2025/05/27 17:18:04 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

static int	handle_eof_case(char *delim)
{
	ft_putstr_fd("minishell: warning: here-document delimited by ",
		STDERR_FILENO);
	ft_putstr_fd("end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delim, STDERR_FILENO);
	ft_putstr_fd("`)\n", STDERR_FILENO);
	return (0);
}

static int	handle_sigint_case(void)
{
	g_shell_state = STATE_HEREDOC_INTERRUPTED;
	return (-1);
}

static int	process_readline_result(char *line, char *delim)
{
	if (!line)
	{
		if (g_shell_state == STATE_EXECUTION_INTERRUPTED)
			return (handle_sigint_case());
		else
			return (handle_eof_case(delim));
	}
	if (g_shell_state == STATE_EXECUTION_INTERRUPTED)
	{
		free(line);
		return (handle_sigint_case());
	}
	return (1);
}

static int	check_delimiter_match(char *line, char *delim)
{
	if (ft_strlen(line) == ft_strlen(delim)
		&& ft_strncmp(line, delim, ft_strlen(delim)) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

int	read_heredoc_lines(int pipefd, char *delim)
{
	char	*line;
	int		result;

	if (!delim)
	{
		ft_putstr_fd("error: delimiter is NULL\n", STDERR_FILENO);
		return (-1);
	}
	while (1)
	{
		if (g_shell_state == STATE_EXECUTION_INTERRUPTED)
			return (-1);
		line = readline("> ");
		result = process_readline_result(line, delim);
		if (result <= 0)
			return (result);
		if (check_delimiter_match(line, delim))
			break ;
		result = write_heredoc_line(pipefd, line);
		if (result < 0)
			return (result);
	}
	return (1);
}
