/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:06:45 by paromero          #+#    #+#             */
/*   Updated: 2025/05/28 16:02:11 by paromero         ###   ########.fr       */
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

// Estados para la variable global g_shell_state
# define STATE_PROMPT_NORMAL 0
# define STATE_PROMPT_INTERRUPTED 1       // SIGINT en prompt/readline
# define STATE_EXECUTING 2                // Comando ejecutándose
# define STATE_EXECUTION_INTERRUPTED 3    // SIGINT durante ejecución
# define STATE_HEREDOC_INTERRUPTED 4      // SIGINT en heredoc

// Declaración de la única variable global para el estado de señales
// Esta se definirá en main.c
// extern volatile sig_atomic_t g_shell_state;

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

typedef struct s_tokens
{
	t_type			type;
	char			*value;
	struct s_tokens	*next;
}	t_tokens;

typedef struct s_ast_args
{
	t_ast		**root;
	t_ast		**cmd;
	t_ast		**last_op;
	t_tokens	*tokens;
}	t_ast_args;

typedef struct s_env
{
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_expansion
{
	char	*line;
	char	*result;
	int		*i;
	int		*j;
}	t_expansion;

typedef struct s_data
{
	char		*line; //- linea comando actual
	char		*prompt; //- Promp de salida de la minishell (ej: minishell> )
	char		*cwd; //- directorio actual
	int			exit; //- Manejo de salida
	int			last_exit_status; //-salida ultimo comando ejecutado ($?)
	int			wstatus;
	int			only_redirections;
	pid_t		pid; //- Proceso hijo y padre
	t_env		*env; //- Puntero a estructuar de env
	t_tokens	*tokens; //- puntero a estructura de tokens
	t_ast		*ast; //- puntero a estructura ast (Abstract Syntax Tree)
	int			input_redir_fd;
	int			output_redir_fd;
	int			heredoc_pipe_fd;
}	t_data;

//* 				INIT					//

//! init.c //
int			init_data(t_data *data, char **env);
int			init_env(t_data *data, char *env[]);
t_env		*create_env_node(const char *value);
void		reset_redirection_fds(t_data *data);
void		close_redirection_fds(t_data *data);
int			apply_redirections_for_builtin(t_data *data);
void		restore_redirections_for_builtin(t_data *data,
				int saved_stdin, int saved_stdout);

//* 				PARSE					//

//! ft_check_syntax.c //
int			check_syntax(char *line);

//! ft_line_utils.c //
char		*deletefirstspaces(char *line);
int			ft_isspace(char	*line);
int			openquotes(char	*line);
int			handle_invslash_pcomma(char *line);
char		*ft_mask_operator(char *str);

//! ft_prompt.c //
int			update_pwd(t_data	*data);

//! ft_parse_utils.c //
size_t		ft_spacestrlen(char *line);
int			ft_dobletype(t_type	type);
char		*check_access(char *cmd);
int			handle_special_chars(char **array, char const *s, size_t *i);

//! ft_quotesplit.c //
char		**ft_quotesplit(char const *s, char c,	t_data	*data);

//! ft_quotesplit_utils.c //
void		process_quotes(const char *s, size_t *i, char *quote);
void		handle_special_operators(const char *s, size_t *i);
int			is_double_operator(const char *s, size_t i);

//! ft_handle_quotes.c //
void		ft_handle_quotes(t_data	*data, char	**matrix);

//! ft_handle_quotes_utils.c //
char		*ft_handle_quote(char *line, char quote_char);
int			ft_quotelen(char	*line, char quote);

//! ft_handle_dollar.c //
char		*ft_handledollar(t_data *data, char *line);
char		*find_envvar(t_data *data, const char *dollarline);

//! ft_handle_dollar_size.c //
int			calculate_expanded_size(t_data *data, char *line);

//! ft_handle_dollar_utils.c //
char		*get_env_value(t_data *data, const char *name);
int			get_var_name_len(const char *str, int start);
char		*extract_var(const char *line, int *i);
void		handle_exit_status(t_data *data, char *result, int *j);

//! parse_path //
void		free_cmd_path(char	**matrix,	char	*cmd);
char		*find_variable_in_env(t_data *data, char *var);
char		*get_cmd_path(t_data *data, char *cmd);

//! ft_tokens.c //
t_tokens	*ft_new_token(char	*str);
int			ft_types(char	*value);
int			ft_tokens(t_data *data, char *str);

//! ft_ast.c //
t_ast		*ft_build_ast(t_data *data, t_tokens *tokens);
void		ft_add_argument(t_ast *cmd_node, char *arg);

//! ft_ast_utils.c //
t_ast		*ft_create_ast_node(t_type type, char *value);
void		handle_redirection(t_ast **root, t_ast **cmd,
				t_ast **last_op, t_tokens *tokens);
void		connect_operator(t_ast **root, t_ast **cmd,
				t_ast **last_op, t_ast *new_op);
void		handle_new_node(t_ast **root, t_ast **cmd,
				t_ast **last_op, t_ast *new_op);
void		init_ast_vars(t_ast **root, t_ast **cmd, t_ast **last_op,
				t_ast_args *args);
void		ft_handle_command_node(t_ast **root, t_ast **cmd,
				t_ast **last_op, t_tokens *tokens);
int			process_token(t_data *data, t_ast_args *args);
int			ft_handle_operator_node(t_data *data, t_ast_args *args);
int			is_red(t_ast **last_op);

//! ft_ast_redirects.c //
int			handle_file_access(t_data *data, char *filename, int type);
int			handle_redirect_checks(t_data *data, t_tokens *tokens);
int			handle_redirect_update(t_ast *current, t_tokens *tokens);
int			handle_redirect_node(t_ast **root, t_ast **last_op,
				t_tokens *tokens);

//! ft_ast_red_utils.c //
int			is_redirect_token(t_tokens *tokens);
int			handle_redirect_operator(t_ast_args *args);
void		handle_redirection(t_ast **root, t_ast **cmd,
				t_ast **last_op, t_tokens *tokens);

//! ft_ast_arg.c //
void		ft_add_argument(t_ast *cmd_node, char *arg);
void		handle_command_as_arg(t_ast **cmd, t_ast **redirect,
				t_tokens *tokens);
void		copy_args(char **new_args, t_ast *cmd_node, int i, char *arg);

//! ft_redin2_cases.c //
int			is_redin2(t_ast **last_operator);
void		redin2(t_ast **current_cmd, t_ast **last_operator,
				t_ast *new_node, t_tokens *tokens);

//! ft_errors.c //
int			fd_error(t_data *data);
int			syntax_error(void);
int			env_error(t_data *data);
char		*handle_invalid_cmd_path(char **cmd_path, char *total_cmd);
void		exit_minishell(t_data *data, const char *error_message,
				int exit_code);

//! ft_free.c //
int			free_while(t_data	*data);
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

//! logical_and_or.c //
void		exec_logical_and(t_data *data, t_ast *node);
void		exec_logical_or(t_data *data, t_ast *node);

//!	exec_utils.c //
char		**list_to_array(t_env *head);
void		free_array(const char **array);
void		analyse_status(t_data *data);
int			handle_process_error(int result, char *error_msg);

//! redirections.c //
void		exec_redirect_out(t_data *data, t_ast *node);
void		exec_redirect_in(t_data *data, t_ast *node);
void		exec_redirect_append(t_data *data, t_ast *node);
int			write_heredoc_line(int pipefd, char *line);

//! heredoc.c //
void		exec_heredoc(t_data	*data, t_ast *node);

//! redirect_utils.c //
t_ast		*find_cmd_node(t_ast *node);
t_ast		*find_last_redirection(t_ast *node);
int			check_file_permissions(t_ast *current, t_data *data);
void		apply_last_redirect(t_ast *redirect, t_ast *cmd, t_data *data);
int			handle_heredoc_result(int result, t_data *data, int pipefd[2],
				t_ast *node);
int			setup_heredoc_pipe(t_data *data, int original_stdin, int pipefd[2]);

//! heredoc_utils.c //
int			write_heredoc_line(int pipefd, char *line);
int			read_heredoc_lines(int pipefd, char *delim);

//* 				BUILTS_IN					//

//!	builtin.c //
int			is_builtins(t_data *data, char *line);

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

//! export_utils.c //
void		print_env_sorted(char **env_matrix);
void		print_export_matrix(char **matrix);
void		sort_matrix(char **matrix);

//! env_builtin.c //
int			print_env(t_env *env);
int			env_builtin(t_data *data);

//! exit_builtin.c //
int			exit_builtin(t_data *data, t_ast *ast);

//! unset_builtint.c //
int			unset_builtin(t_data *data);

//! last_exit_status_builtin.c //
int			last_cmd_status(t_data	*data);

//! printfuncs //
// int			print_tokens(t_tokens *token);
// void		print_type(t_tokens *token);

//! signals.c //
void		handle_sigint(int sig);
void		handle_sigquit(int sig);
void		setup_signals(void);
void		setup_signals_for_child(void);

//! Minishell_Function //
int			minishell(char **env);
int			line_syntax(t_data *data);

//! main_utils.c //
int			handle_signal_states(t_data *data);
int			handle_readline_result(t_data *data);
int			is_builtin_command(char *cmd);
int			is_only_empty_var(char *line);
int			is_empty_var_with_cmd(t_data *data, char *line);
void		execute_builtin_with_redirections(t_data *data);

//! main_checks.c //
int			check_line_errors(t_data *data);

#endif
/**
 * - explicación
 * * importante
 * ! alertas
 * ? dudas
 * TODO por hacer
 * DONE hecho
 */