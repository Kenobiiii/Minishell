/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:06:45 by paromero          #+#    #+#             */
/*   Updated: 2025/02/06 12:53:12 by anggalle         ###   ########.fr       */
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


# define ANSI_COLOR_GREEN "\e[1;96m"
# define ANSI_COLOR_RED "\e[1;91m"
# define ANSI_COLOR_RESET "\e[0m"
# define ANSI_COLOR_BLUE "\e[1;34m"

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
	int			wstatus;
	pid_t		pid; //- Proceso hijo y padre
	t_env		*env; //- Puntero a estructuar de env
	t_tokens	*tokens; //- puntero a estructura de tokens
	t_ast		*ast; //- puntero a estructura ast (Abstract Syntax Tree)	
}	t_data;

extern sig_atomic_t	g_sigint_received;

//* 				INIT					//

//! init.c //
int			init_data(t_data *data, char **env);
int			init_env(t_data *data, char *env[]);
t_env		*create_env_node(const char *value);

//* 				PARSE					//

//! ft_line_utils.c //
char		*deletefirstspaces(char *line);
int			ft_isspace(char	*line);
int			openquotes(char	*line);
int			handle_invslash_pcomma(char *line);

//! ft_parse_utils.c //
size_t		ft_spacestrlen(char *line);
int			ft_dobletype(t_type	type);
int			update_pwd(t_data	*data);

//! ft_quotesplit.c //
char		**ft_quotesplit(char const *s, char c,	t_data	*data);

//! ft_handle_quotes.c //
void		ft_handle_quotes(t_data	*data, char	**matrix);

//! ft_handle_dollar.c //
char		*ft_handledollar(t_data *data, const char *line);

//! parse_path //
void		free_cmd_path(char	**matrix,	char	*cmd);
char		*find_path_in_env(t_data *data);
char		*find_variable_in_env(t_data *data, char *var);
char		*get_cmd_path(t_data *data, char *cmd);

//! ft_tokens.c //
t_tokens	*ft_new_token(char	*str);
int			ft_types(char	*value);
int			ft_tokens(t_data *data, char *str);

//! ft_ast.c //
t_ast		*ft_build_ast(t_tokens *tokens);
void		ft_add_argument(t_ast *cmd_node, char *arg);

//! ft_redin2_cases.c //
int			is_redin2(t_ast **last_operator);
void		redin2(t_ast **current_cmd, t_ast **last_operator,
				t_ast *new_node, t_tokens *tokens);

//! ft_errors.c //
int			syntax_error(void);
char		*handle_invalid_cmd_path(char **cmd_path, char *total_cmd);

//! ft_free.c //
void		free_innerwhile(t_data	*data);
void		free_minishell(t_data	*data);

//! ft_free_parse.c //
void		free_matrix(char **array);
void		ft_free_tokens(t_tokens *tokens);
void		ft_free_error_token(t_data	*data, char **result);
void		ft_free_env(t_env	*env);
void		ft_free_ast(t_ast	*ast);

//* 				EXECUTION					//

//! exec.c //
void		exec_func(t_data *data);
void		exec_ast(t_data *data, t_ast *node);
void		exec_simple_cmd(t_data *data, t_ast *node);
void		exec_logical_and(t_data *data, t_ast *node);
void		exec_logical_or(t_data *data, t_ast *node);

//!	exec_utils.c //
char		**list_to_array(t_env *head);
void		free_array(const char **array);

//! redirections.c //
void		exec_redirect_out(t_data *data, t_ast *node);
void		exec_redirect_in(t_data *data, t_ast *node);
void		exec_redirect_append(t_data *data, t_ast *node);
void		exec_heredoc(t_data	*data, t_ast *node);

//* 				BUILTS_IN					//

//!	builtin.c //
int			is_builtins(t_data *data);

//! echo_builtin.c //
int			echo_builtin(t_data *data);

//! pwd_builtint.c //
int			pwd_builtin(t_data *data);

//! cd_builtint.c //
int			cd_builtin(t_data *data);

//! exit_builtin.c //
void		exec_pipe(t_data *data, t_ast *node);

//! export_builtint.c //
int			export_builtin(t_data *data);
void		print_env_sorted(char **env_matrix);
void		print_export_matrix(char **matrix);
void		sort_matrix(char **matrix);

//! env_builtin.c //
int			print_env(t_env *env);
int			env_builtin(t_data *data);

//! exit_builtin.c //
int			exit_builtin(t_data *data);

//! unset_builtint.c //
int			unset_builtin(t_data *data);

//! printfuncs //
// int			print_tokens(t_tokens *token);
// void		print_type(t_tokens *token);

//! signals.c //
void		handle_sigint(int sig);
void		setup_signals(void);

//! Minishell_Function //
int			minishell(char **env);

#endif
/**
 * - explicación
 * * importante
 * ! alertas
 * ? dudas
 * TODO por hacer
 * DONE hecho
 */