/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:06:45 by paromero          #+#    #+#             */
/*   Updated: 2025/01/21 14:07:59 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <signal.h>
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
typedef struct s_ast
{
	t_type			type;
	char			*value;
	char			**args;
	struct s_ast	*left;
	struct s_ast	*right;
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
char		*ft_delete_spaces(char *line);
int			ft_syntax(t_data *data);

//! ft_free_parse.c //
void		ft_free_split(char **split);
void		ft_free_tokens(t_tokens *tokens);
void		ft_free_error_token(t_data	*data, char **result);

//! ft_parse_utils.c //
size_t		ft_spacestrlen(char *line);
t_tokens	*ft_new_token(char	*str);
int			ft_dobletype(t_type	type);

//! ft_ast.c //
t_ast		*ft_build_ast(t_tokens *tokens);
void		ft_add_argument(t_ast *cmd_node, char *arg);

//! ft_signals.c //

//! parse_path //
char	*find_path_in_env(t_data *data);
char	*get_cmd_path(t_data *data, char *cmd);

//! exec.c //
void	create_command(char **cmd, char *argv);
void analyse_status(int wstatus);
void exec_func(t_data *data);

//!	exec_utils.c //
const char **list_to_array(t_env *head);
void free_array(const char **array); 

#endif
/**
 * - explicación
 * * importante
 * ! alertas
 * ? dudas
 * TODO por hacer
 * DONE hecho
 */