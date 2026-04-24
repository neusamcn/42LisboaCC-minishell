/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:24:22 by megi              #+#    #+#             */
/*   Updated: 2026/04/22 21:12:11 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

//printing msges to stdout 
// wildcart character * to output multiple files (ike cat hello*)

// print everything that i pass as a third args untill any redirections
// untill a newline

// getchar and putchar till receive a newline
int myecho(t_cmd_line *cmd, char **envp)
{
	int i;
	int newline;

	(void)envp;
	i = 1;
	newline = 1;
	while (cmd->cmds[i] && ft_strcmp(cmd->cmds[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->cmds[i])
	{
		p("%s", cmd->cmds[i]);
		if (cmd->cmds[i + 1])
			p(" ");
		i++;
	}
	if (newline)
		p("\n");
	return (0);
>>>>>>> 13821a44efc7e1df35d8db44c41dd885ef316a3d
}
