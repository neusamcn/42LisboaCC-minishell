/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 22:25:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/19 18:34:58 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*System Errors. Usjng for: malloc, open, pipe, dup2, fork*/
void	print_err_msg(char *my_msg)
{
	ft_putstr_fd(PALE_VIOLET_RED, 2);
	ft_putstr_fd(my_msg, 2);
	ft_putstr_fd(COLOR_RESET": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

/* User Command Errors. Useful for cmn not found, permissions, syntax errors */
int	mndp_exec_error(char *cmd)
{
	if (errno == EACCES)
		mndp_log_err("Permission denied", cmd);
	else if (errno == ENOENT)
		mndp_log_err("No such file or directory", cmd);
	else if (errno == EISDIR)
		mndp_log_err("Is a directory", cmd);
	else
		mndp_log_err(strerror(errno), cmd);
	return (127);
}

/*Syntax err, custom messages, bltns*/
int	mndp_log_err(char *msg, char *cmd)
{
	ft_putstr_fd(PALE_VIOLET_RED, 2);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(COLOR_RESET, 2);
	ft_putchar_fd('\n', 2);
	return (true);
}

void	syntax_err_msg(char *err_token)
{
	// TODO: should it output to STDERR ? use print_err_msg?
	ft_printf("-bash shelly: syntax error near unexpected token `%s'\n",
		err_token);
}
