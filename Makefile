NAME = minishell
HEADER = include/minishell.h

LIBFT_DIR	= libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJ_DIR = obj

CC = cc
CFLAGS = -Wall -Werror -Wextra -Iinclude
LDFLAGS = -L/opt/homebrew/opt/readline/lib
CPPFLAGS = -I/opt/homebrew/opt/readline/include
LDLIBS = -lreadline

SRCS = $(SRC_DIR)/main.c \
		$(SRC_DIR)/execution/ex_path_ecution.c \
		$(SRC_DIR)/execution/execution.c \
		$(SRC_DIR)/execution/ex_pipes_ecution.c \
		$(SRC_DIR)/execution/ex_signals_ecution.c \
		$(SRC_DIR)/execution/ex_frees_ecution.c \
		$(SRC_DIR)/execution/errors_to_del.c \
		$(SRC_DIR)/execution/ex_signals2_ecution.c \
		$(SRC_DIR)/execution/ex_redir_ecution.c \
		$(SRC_DIR)/execution/ex_utils_ecution.c \
		$(SRC_DIR)/builtins/ex_bltn_ecution.c \


OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(LIBFT) $(OBJ_DIR) $(NAME)
	@echo "Minishell is ready."

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -Llibft -lft $(LDFLAGS) $(LDLIBS) -o $(NAME)
	@echo "$(NAME) compiled.\n"

$(LIBFT):
	@$(MAKE) -s -C libft libft.a

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	@-rm -f $(OBJS)
	@-rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	@echo "All object files and libraries removed.\n"

fclean: clean
	@-rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "Executables, object files and libraries removed.\n"

re: fclean all

.PHONY: all clean fclean re readme