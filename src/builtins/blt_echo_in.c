/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:24:22 by megi              #+#    #+#             */
/*   Updated: 2026/05/25 17:54:59 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

//printing mss to stdout 
// wildcart character * to output multiple files (ike cat hello*)
// print everything that i pass as a third args untill any redirections
// untill a newline
// getchar and putchar till receive a newline

int	myecho(t_cmd_line *cmd, t_shelly *shelly)
{
	int	i;
	int	if_newline_exists;

	(void)shelly->envp;
	i = 1;
	if_newline_exists = 1;
	while (cmd->cmds[i] && ft_strcmp(cmd->cmds[i], "-n") == SHELLYMATES)
	{
		if_newline_exists = 0;
		i++;
	}
	while (cmd->cmds[i])
	{
		ft_printf("%s", cmd->cmds[i]);
		if (cmd->cmds[i + 1])
			ft_printf(" ");
		i++;
	}
	if (if_newline_exists)
		ft_printf("\n");
	return (OK);
}
