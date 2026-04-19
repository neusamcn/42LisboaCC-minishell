/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 22:25:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/10 13:28:41 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../include/minishell.h"

void	print_err_msg(char *my_msg)
{
	ft_putstr_fd(PALE_VIOLET_RED, 2);
	ft_putstr_fd(my_msg, 2);
	ft_putstr_fd(COLOR_RESET": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

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
