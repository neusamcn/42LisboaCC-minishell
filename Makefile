#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: megi <megi@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/27 21:38:07 by ncruz-ne          #+#    #+#              #
#    Updated: 2026/04/02 17:45:38 by megi             ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = minishell
HEADER = include/minishell.h

LIBFT = libft/libft.a

SRC_DIR = src
OBJ_DIR = obj

CC = cc
CFLAGS = -Wall -Werror -Wextra -Iinclude
LDFLAGS = -L/opt/homebrew/opt/readline/lib
CPPFLAGS = -I/opt/homebrew/opt/readline/include
LDLIBS = -lreadline

SRCS = $(SRC_DIR)/main.c \
		$(SRC_DIR)/execution/path.c \
		$(SRC_DIR)/execution/execution.c \
		$(SRC_DIR)/execution/pipes.c \
		$(SRC_DIR)/execution/signals.c \
		$(SRC_DIR)/execution/errors_to_del.c \
		$(SRC_DIR)/execution/parsing_to_delete.c \

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
	@$(MAKE) -C libft clean
	@echo "All object files and libraries removed.\n"

fclean: clean
	@-rm -f $(NAME)
	@$(MAKE) -C libft fclean
	@echo "Executables, object files and libraries removed.\n"

re: fclean all

.PHONY: all clean fclean re readme