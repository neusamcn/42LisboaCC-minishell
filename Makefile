# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/27 21:38:07 by ncruz-ne          #+#    #+#              #
#    Updated: 2026/01/27 21:47:08 by ncruz-ne         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = include/minishell.h

LIBFT = libft/libft.a

SRC_DIR = src
OBJ_DIR = obj

CC = cc
CFLAGS = -Wall -Werror -Wextra -Iinclude

SRCS = $(SRC_DIR)/main.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(LIBFT) $(OBJ_DIR) $(NAME)
	@echo "Minishell is ready."


$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -o $(NAME)
	@echo "$(NAME) compiled.\n"

$(LIBFT):
	@$(MAKE) -C libft libft.a
	@echo "$(LIBFT) compiled.\n"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(OBJ_DIR) directory created.\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$@ created.\n"


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
