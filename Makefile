LIB = ar rcs
RM = rm -f

CC = clang
CFLAGS = -Wall -Werror -Wextra

USER = paromero
OBJ_DIR = objs
SRCS = src/main.c \
		src/main_utils.c \
		src/main_checks.c \
		src/init/init.c \
		src/parse/ft_check_syntax.c \
		src/parse/ft_line_utils.c \
		src/parse/ft_parse_utils.c \
		src/parse/ft_prompt.c \
		src/parse/ft_handle_dollar.c \
		src/parse/ft_handle_dollar_size.c \
		src/parse/ft_handle_dollar_utils.c \
		src/parse/ft_handle_quotes.c \
		src/parse/ft_handle_quotes_utils.c \
		src/parse/ft_parse_path.c \
		src/parse/ft_quotesplit_utils.c \
		src/parse/ft_quotesplit.c \
		src/parse/ft_tokens.c \
		src/parse/ft_ast.c \
		src/parse/ft_ast_utils.c \
		src/parse/ft_ast_arg.c \
		src/parse/ft_ast_redirects.c \
		src/parse/ft_ast_red_utils.c \
		src/errors/ft_errors.c \
		src/parse/ft_redin2_cases.c \
		src/parse/ft_free_parse.c \
		src/parse/ft_free.c \
		src/execution/exec.c \
		src/execution/exec_utils.c \
		src/execution/logical_and_or.c \
		src/execution/signals.c \
		src/execution/pipe.c \
		src/execution/redirections.c \
		src/execution/redirect_utils.c \
		src/execution/heredoc.c \
		src/execution/heredoc_utils.c \
		src/builts_ins/echo_builtin.c \
		src/builts_ins/builtin.c \
		src/builts_ins/pwd_builtin.c \
		src/builts_ins/cd_builtin.c \
		src/builts_ins/export_builtin.c \
		src/builts_ins/export_utils.c \
		src/builts_ins/unset_builtin.c \
		src/builts_ins/env_builtin.c \
		src/builts_ins/exit_builtin.c \
		src/builts_ins/last_exit_status_builtin.c

OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))

NAME = minishell

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I./$(LIBFT_DIR) -I/usr/include/readline
LIBS = -L./$(LIBFT_DIR) -lft -lreadline -lncurses

GREEN = \033[1;32m
RESET = \033[0m

all: $(LIBFT) $(OBJ_DIR) $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling objects...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Executable created: $(NAME)$(RESET)"

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)$< compiled!$(RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	@echo "$(GREEN)Building libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "$(GREEN)Cleaning...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJS)
	@echo "$(GREEN)Clean complete.$(RESET)"

fclean: clean
	@echo "$(GREEN)Deleting $(NAME)...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -rf $(OBJ_DIR)
	@$(RM) $(NAME)
	@echo "$(GREEN)Deleted $(NAME)$(RESET)"

re: fclean all

.PHONY: all clean fclean re