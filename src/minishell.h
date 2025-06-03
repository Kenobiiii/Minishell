/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:06:45 by paromero          #+#    #+#             */
/*   Updated: 2025/06/03 18:24:27 by paromero         ###   ########.fr       */
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

# define ANSI_COLOR_GREEN "\001\e[1;96m\002"
# define ANSI_COLOR_RED "\001\e[1;91m\002"
# define ANSI_COLOR_RESET "\001\e[0m\002"
# define ANSI_COLOR_BLUE "\001\e[1;34m\002"

# define STATE_PROMPT_NORMAL 0
# define STATE_PROMPT_INTERRUPTED 1
# define STATE_EXECUTING 2
# define STATE_EXECUTION_INTERRUPTED 3
# define STATE_HEREDOC_INTERRUPTED 4

typedef enum e_type
{
	CMD,
	REDIRECT_OUT,
	REDIRECT_IN,
	REDIN2,
	REDOUT2,
	PIPE
}	t_type;

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

typedef struct s_finalize_params
{
	t_ast	*original_cmd;
}	t_finalize_params;

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
	char		*line;
	char		*prompt;
	char		*cwd;
	int			exit;
	int			wstatus;
	int			only_redirections;
	t_env		*env;
	t_tokens	*tokens;
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
void		close_redirection_fds(t_data *data);
int			apply_redirections_for_builtin(t_data *data);
void		restore_redirections_for_builtin(t_data *data,
				int saved_stdin, int saved_stdout);

//* 				PARSE					//

//! ft_check_syntax.c //
int			check_syntax(char *line);

//! ft_syntax_helpers.c //
int			check_operator_syntax(char *line, int *i, char operator);
int			check_logical_operators(char *line, int i);
void		handle_syntax_quotes(char *line, int *i, int *in_quote,
				char *quote_type);
int			check_syntax_loop(char *line, int *i, int *in_quote,
				char *quote_type);

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
void		ft_process_all_quotes(t_data	*data, char	**matrix);
int			ft_find_quotes_in_line(t_data *data, char **matrix, int count_x);

//! ft_handle_quotes_utils.c //
char		*ft_handle_type_quote(char *line, char quote_char);
int			ft_quotelen(char	*line, char quote);
void		ft_process_all_quotes(t_data	*data, char	**matrix);

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

//! ft_ast_red_helpers.c //
t_ast		*create_redirection_node(t_ast **cmd, t_ast **last_op,
				t_tokens *tokens);
void		cleanup_unused_node(t_ast *new_node, t_ast **last_op);
void		finalize_redirection_ast(t_ast **root, t_ast **cmd,
				t_ast **last_op, t_finalize_params *params);
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
char		*handle_invalid_cmd_path(char **cmd_path, char *total_cmd);
void		exit_minishell(t_data *data, const char *error_message,
				int exit_code);

//! ft_free.c //
int			free_while(t_data	*data);
void		free_minishell(t_data	*data);
void		ft_free_command_node(t_ast	*node, int flag);

//! ft_free_parse.c //
void		free_matrix(char **array);
void		ft_free_tokens(t_tokens *tokens);
void		ft_free_error_token(t_data	*data, char **result);
void		ft_free_env(t_env	*env);
void		ft_free_ast(t_ast	*ast);

//* 				EXECUTION					//

//! exec.c //
void		exec_ast(t_data *data, t_ast *node);
void		exec_simple_cmd(t_data *data, t_ast *node);

//! variable_expansion.c //
void		expand_command_variables(t_data *data, t_ast *node);

//!	exec_utils.c //
char		**list_to_array(t_env *head);
void		free_array(const char **array);
void		analyse_status(t_data *data);
int			handle_process_error(int result, char *error_msg);

//! redirections.c //
void		exec_redirect_out(t_data *data, t_ast *node);
void		exec_redirect_in(t_data *data, t_ast *node);
void		exec_redirect_append(t_data *data, t_ast *node);

//! redirections_utils.c //
int			write_heredoc_line(int pipefd, char *line);
t_ast		*find_cmd_node(t_ast *node);
int			count_output_redirections(t_ast *node);
void		collect_redirections(t_ast *node, t_ast **redirections, int *index);
int			open_redirection_file(t_ast *redirection);

//! redirections_multi.c //
void		process_multiple_output_redirections(t_data *data, t_ast *node);

//! redirections_helpers.c //
int			validate_input_redirect_node(t_data *data, t_ast *node);
void		execute_redirect_helper(t_data *data, t_ast *node, int flags);
int			handle_input_redirect_open(t_data *data, char *filename);

//! redirections_process.c //
int			process_redirection_files(t_data *data, t_ast **redirections,
				int count);
void		execute_command_after_redirections(t_data *data, t_ast *node);

//! heredoc.c //
void		exec_heredoc(t_data	*data, t_ast *node);

//! redirect_utils.c //
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

//! export_utils.c //
void		print_env_sorted(char **env_matrix);
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

//! signals.c //
void		handle_sigint(int sig);
void		handle_sigquit(int sig);
void		setup_signals(void);
void		configure_signals(int mode);

//! Minishell_Function //
int			minishell(char **env);
int			line_syntax(t_data *data);

//! main_utils.c //
int			handle_signal_states(t_data *data);
int			handle_readline_result(t_data *data);
int			is_builtin_command(char *cmd);
void		execute_builtin_with_redirections(t_data *data);

//! main_checks.c //
int			check_line_errors(t_data *data);
int			is_only_empty_var(char *line);
int			is_empty_var_with_cmd(t_data *data, char *line);

#endif
/**
 * * Important
 * ! Alerts
 * ? Explanation
 * TODO To Do
 * DONE Done
 */