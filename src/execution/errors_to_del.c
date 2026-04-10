#include "../../include/minishell.h"

//FILE TO DELETE BUT PREVIOUSLY DISCUSS W NEUSA
void	print_err_msg(char *my_msg)
{
	ft_putstr_fd(PALE_VIOLET_RED, 2);
	ft_putstr_fd(my_msg, 2);
	ft_putstr_fd(COLOR_RESET, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
    exit (1);
}

//TODO: SHOW IT TO NEUAAAS
void	p_log_err(char *cmd, char *msg)
{
	ft_putstr_fd(PALE_VIOLET_RED, 2);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(COLOR_RESET, 2);
	ft_putchar_fd('\n', 2);
    exit (1);
}