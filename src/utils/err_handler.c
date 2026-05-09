/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/05/09 14:57:51 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	mndp_log_err(char *msg, char *cmd)
{
	ft_putstr_fd(PALE_VIOLET_RED, 2);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(COLOR_RESET, 2);
    return (true);
}

void	syntax_err_msg(char *err_token)
{
	// TODO: should it output to STDERR ? use print_err_msg?
	p("-bash shelly: syntax error near unexpected token `%s'\n",
		err_token);
}
