/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:06:45 by paromero          #+#    #+#             */
/*   Updated: 2024/11/05 19:36:33 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <curses.h>
# include <ncurses.h>

# define WRONG_INPUT 1
# define MALLOC_ERROR 2

typedef struct s_env
{
	char  	*value;
	struct s_env *next;
}	t_env;

typedef struct s_data
{
	char	*line //- linea comando actual
	char	*prompt //- Promp de salida de la minishell (ej: minishell> )
	char	*cwd //- directorio actual
	int		last_exit_status //- salida ultimo comando ejecutado ($?)
	int		signal_received //- manejo de las señales
	pid_t	pid //- Proceso hijo y padre
	t_env	*env //- Puntero a estructuar de env
	t_token	*tokens //- puntero a estructura de tokens
	t_ast	*ast //- puntero a estructura ast (Abstract Syntax Tree)
	
}	t_data;

#endif

/**
 * - explicación
 * * Importante
 * ! alertas
 * ? Dudas
 * TODO por hacer
 */