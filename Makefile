LIB = ar rcs
RM = rm -f

CC = clang
CFLAGS = -Wall -Werror -Wextra

USER = paromero
SRC_DIR = src
OBJ_DIR = objs
SRCS = src/main.c #! no se permite wildcard
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

NAME = minishell

# Colores
GREEN = \033[1;32m
RESET = \033[0m

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling objects...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Executable created: $(NAME)$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)$< compiled!$(RESET)"

# Crea la carpeta objs si no existe
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "$(GREEN)Cleaning...$(RESET)"
	@$(RM) $(OBJS)
	@echo "$(GREEN)Clean complete.$(RESET)"

fclean: clean
	@echo "$(GREEN)Deleting $(NAME)...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(RM) $(NAME)
	@echo "$(GREEN)Deleted $(NAME)$(RESET)"

re: fclean all

.PHONY: all clean fclean re
