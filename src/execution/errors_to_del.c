/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_to_del.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 01:28:50 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void	p_log_err(char *msg)
{
	ft_putstr_fd(PALE_VIOLET_RED, 2);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(COLOR_RESET, 2);
	ft_putchar_fd('\n', 2);
}
