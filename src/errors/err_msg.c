/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 22:25:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/04 15:36:23 by ncruz-ne         ###   ########.fr       */
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
