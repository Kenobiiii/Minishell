/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:06:45 by paromero          #+#    #+#             */
/*   Updated: 2024/11/28 19:58:48 by paromero         ###   ########.fr       */
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
# include <linux/limits.h>
# include "../libft/libft.h"

typedef enum e_type
{
	CMD,
	REDIRECT_OUT,
	REDIRECT_IN,
	REDIN2,
	REDOUT2,
	PIPE,
	AND,
	OR
}	t_type;
//- struct para guardar los comandos simples
typedef struct s_cmd
{
	char	**args;
}	t_cmd;

//- struct para manejar la redirección
typedef struct s_redirect
{
	t_type			type; //- guarda el tipo de redirección que es
	struct s_ast	*left; //- guarda el comando de la izquierda
	char			*file; //- guarda el archivo al que se va a añadir
}	t_redirect;

typedef struct s_ast
{
	t_type			type;
	struct s_ast	*left;
	struct s_ast	*right;
	union
	{
		t_cmd		cmd;
		t_redirect	redirect;
	} u_uniond;
}	t_ast;

/**
 *- Uniones:
 *- son variables que comparten la misma memoria para así ahorrarla
 *- si una se modifica la otra también, útil para casos donde depende
 *- del tipo se necesita una cosa u otra
 */

typedef struct s_tokens
{
	t_type			type;
	char			*value;
	struct s_tokens	*next;
}	t_tokens;

typedef struct s_env
{
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char		*line; //- linea comando actual
	char		*prompt; //- Promp de salida de la minishell (ej: minishell> )
	char		*cwd; //- directorio actual
	int			exit; //- Manejo de salida
	int			last_exit_status; //-salida ultimo comando ejecutado ($?)
	pid_t		pid; //- Proceso hijo y padre
	t_env		*env; //- Puntero a estructuar de env
	t_tokens	*tokens; //- puntero a estructura de tokens
	t_ast		*ast; //- puntero a estructura ast (Abstract Syntax Tree)	
}	t_data;

//! init.c //
int			init_data(t_data *data, char **env);
int			init_env(t_data *data, char *env[]);
t_env		*create_node(const char *valor);

//! parse.c //
int			ft_types(char	*value);
int			ft_tokens(t_data *data, char *str);
char		*ft_spaces(char *line);
int	ft_syntax(t_data *data);

//! ft_free_parse.c //
void		ft_free_split(char **split);
void		ft_free_tokens(t_tokens *tokens);
void		ft_free_error_token(t_data	*data, char **result);

//! ft_parse_utils.c //
size_t		ft_spacestrlen(char *line);
t_tokens	*ft_new_token(char	*str);
int			ft_dobletype(t_type	type);

#endif
/**
 * - explicación
 * * importante
 * ! alertas
 * ? dudas
 * TODO por hacer
 * DONE hecho
 */