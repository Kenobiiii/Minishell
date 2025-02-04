LIB = ar rcs
RM = rm -f

CC = clang
CFLAGS = -Wall -Werror -Wextra

USER = paromero
OBJ_DIR = objs
SRCS = src/main.c \
		src/init/init.c \
		src/parse/ft_tokens.c \
		src/parse/ft_free_parse.c \
		src/parse/ft_parse_utils.c \
		src/parse/ft_ast.c \
		src/execution/exec.c \
		src/parse/ft_parse_path.c \
		src/execution/exec_utils.c \
		src/builts_ins/echo_builtin.c \
		src/builts_ins/builtin.c \
		src/builts_ins/pwd_builtin.c \
		src/parse/ft_quotesplit.c \
		src/builts_ins/cd_builtin.c \
		src/builts_ins/export_builtin.c \
		src/builts_ins/unset_builtin.c \
		src/builts_ins/env_builtin.c \
		src/builts_ins/exit_builtin.c \
		src/execution/signals.c \
		src/execution/pipe.c \
		src/execution/redirections.c \
		src/parse/ft_errors.c \
		src/prints_funcs.c \
		src/parse/ft_handle_dollar.c \
		src/parse/ft_handle_quotes.c \
		src/parse/ft_redin2_cases.c \
		src/parse/ft_free.c

OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))  # src/main.c → objs/main.o

NAME = minishell

# Configuración para libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I./$(LIBFT_DIR) -I/usr/include/readline
LIBS = -L./$(LIBFT_DIR) -lft -lreadline -lncurses

# Colores
GREEN = \033[1;32m
RESET = \033[0m

all: $(LIBFT) $(OBJ_DIR) $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling objects...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Executable created: $(NAME)$(RESET)"

# Regla corregida: sin SRC_DIR y creando subdirectorios
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)  # Crea subdirectorios (ej: objs/parse/)
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