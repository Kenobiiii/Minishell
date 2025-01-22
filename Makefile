LIB = ar rcs
RM = rm -f

CC = clang
CFLAGS = -g -Wall -Werror -Wextra

USER = paromero
SRC_DIR = src
OBJ_DIR = objs
SRCS = src/main.c src/init.c src/parse.c src/ft_free_parse.c src/ft_parse_utils.c src/ft_signals.c src/ft_ast.c src/exec.c src/parse_path.c src/exec_utils.c src/echo_builtin.c src/builtin.c src/pwd_builtin.c#! no se permite wildcard
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
