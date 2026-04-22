/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_bltn_ecution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 21:00:17 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 23:37:11 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int are_you_builtin(t_cmd_line *cmd_line)
{
	if (!cmd_line || !cmd_line->cmds || !cmd_line->cmds[0])
        return (EXTRENAL);
	if ((!ft_strcmp(cmd_line->cmds[0], CD)) || (!ft_strcmp(cmd_line->cmds[0], ECHO)) || (!ft_strcmp(cmd_line->cmds[0], EXIT)) ||
		(!ft_strcmp(cmd_line->cmds[0], PWD)) || (!ft_strcmp(cmd_line->cmds[0], ENV)) || (!ft_strcmp(cmd_line->cmds[0], EXPORT)) ||
		(!ft_strcmp(cmd_line->cmds[0], UNSET)))
		return (BUILTINS);
	return (EXTRENAL);
}


int run_bltn(t_bltn *names, char **envp)
{
	char *whoareyou;

	while (1)
	{
		if (!ft_strcmp(whoareyou, CD))
		{
			myecho();
		}
	}
	// FORK->EXEC
}