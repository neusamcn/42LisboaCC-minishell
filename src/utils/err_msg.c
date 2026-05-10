/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 22:25:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/11 00:00:39 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_err_msg(char *my_msg)
{
	ft_putstr_fd(PALE_VIOLET_RED, 2);
	ft_putstr_fd(my_msg, 2);
	ft_putstr_fd(COLOR_RESET": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

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
