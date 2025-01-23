LIB = ar rcs
RM = rm -f

CC = clang
CFLAGS = -g -Wall -Werror -Wextra

USER = paromero
SRC_DIR = src src/init
OBJ_DIR = objs
SRCS = src/main.c src/init/init.c src/parse/ft_tokens.c src/parse/ft_free_parse.c src/parse/ft_parse_utils.c src/parse/ft_ast.c src/execution/exec.c src/parse/ft_parse_path.c src/execution/exec_utils.c src/builts_ins/echo_builtin.c src/builts_ins/builtin.c src/builts_ins/pwd_builtin.c src/parse/ft_quotesplit.c#! no se permite wildcard
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<...$(RESET)"
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
