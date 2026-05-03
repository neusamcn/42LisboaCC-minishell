NAME 			= minishell

LIBFT_DIR		= libft
LIBFT_A			= $(LIBFT_DIR)/libft.a

SRC_DIR 		= src
OBJ_DIR 		= obj

PARSING_SRCS	= $(addprefix $(SRC_DIR)/parsing/, ft_wrappers.c init.c shellyenvp.c prompt.c signals.c)
#					tokenize.c syntax_utils.c syntax_check.c)
EXECUTION_SRCS	= $(addprefix $(SRC_DIR)/execution/, path.c exec_utils.c)
# 					execution.c pipes.c parsing_to_delete.c signals.c # Milena, I did this to be able to compile for test
ERRORS_SRCS		= $(addprefix $(SRC_DIR)/errors/, err_msg.c)
SRCS 			= $(SRC_DIR)/main.c $(PARSING_SRCS) $(EXECUTION_SRCS) $(ERRORS_SRCS)
OBJS 			= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

HEADERS 		= $(addprefix include/, minishell.h flair.h parsing.h execution.h)

CC 				= cc
CPPFLAGS 		= -Iinclude
CFLAGS 			= -Wall -Werror -Wextra -D_POSIX_C_SOURCE=200809L
LDFLAGS 		= -L$(LIBFT_DIR)
LDLIBS 			= -lft -lreadline
RM 				= rm -f

TEST_DIR		= test_logs
ESC             := $(shell printf '\033')
COLOR_RESET     := $(ESC)[0m
# DARK_VIOLET     := $(ESC)[38;2;148;0;211m
GREEN           := $(ESC)[32m
# RED             := $(ESC)[31m
# BRIGHT_GREEN    := $(ESC)[92m
# LIGHT_GREEN     := $(ESC)[38;2;144;238;144m
# SPRING_GREEN    := $(ESC)[38;2;0;255;127m
# SOFT_LIME       := $(ESC)[38;2;180;255;180m

SUCCESS         := $(ESC)[38;2;144;238;144m
SUCCESS_BOLD    := $(ESC)[38;2;0;255;127m
INFO            := $(ESC)[38;2;173;216;230m
PATH_COLOR      := $(ESC)[38;2;221;160;221m
# WARNING         := $(ESC)[38;2;255;215;0m
# ERROR_COLOR     := $(ESC)[38;2;255;127;127m
# MUTED           := $(ESC)[38;2;190;190;190m


all: $(NAME)

# TODO: can we hide this libft anouncement? 

$(NAME): $(LIBFT_A) $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
	@printf "%b compiled.\n" "$(GREEN)$@$(COLOR_RESET)"

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) $(notdir $@)
	@printf "%b compiled.\n" "$(SUCCESS_BOLD)$@$(COLOR_RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	@printf "%b created.\n" "$(SUCCESS)$@$(COLOR_RESET)"

$(TEST_DIR):
	@mkdir -p $@
	@printf "%b created.\n" "$(SUCCESS)$@$(COLOR_RESET)"

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@printf "$(INFO)All object files and libraries removed.$(COLOR_RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@printf "$(INFO)Executables, object files and libraries removed.$(COLOR_RESET)\n"

re: fclean all

test: re $(TEST_DIR)
	@valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all -s \
	--log-file=$(TEST_DIR)/valg_out.log ./$(NAME)
	@echo "Valgrind run concluded. See output in $(PATH_COLOR)$(TEST_DIR)/valg_out.log$(COLOR_RESET)"

vsupp: re $(TEST_DIR)
	@valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all -s --suppressions=$(TEST_DIR)/valg_supp_readline.supp \
	--log-file=$(TEST_DIR)/valg_supp_out.log ./$(NAME)
	@echo "Valgrind run concluded. See output in $(PATH_COLOR)$(TEST_DIR)/valg_sup_out.log$(COLOR_RESET)"

testset: re $(TEST_DIR)
	@valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all --gen-suppressions=all --num-callers=30 \
	./$(NAME) 2> $(TEST_DIR)/valgrind.raw
	@echo "Valgrind suppression run concluded. See output in $(PATH_COLOR)$(TEST_DIR)/valg_supression_out.log$(COLOR_RESET)"

tclean:
	@$(RM) -r $(TEST_DIR)

# TODO: add make norme rule to download most updated Norme pdf

.PHONY: all clean fclean re test tclean testset vsupp
