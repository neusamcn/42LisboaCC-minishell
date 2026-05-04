/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_run_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 21:00:17 by megi              #+#    #+#             */
/*   Updated: 2026/05/04 17:03:31 by megi             ###   ########.fr       */
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

int r_bltn(t_cmd_line *cmd_line, t_shelly *shelly)
{
    if (!cmd_line || !cmd_line->cmds || !cmd_line->cmds[0])
        return (1);
    if (!ft_strcmp(cmd_line->cmds[0], ECHO))
        return (myecho(cmd_line, shelly));
    if (!ft_strcmp(cmd_line->cmds[0], ENV))
        return (myenv(cmd_line, shelly));
    if (!ft_strcmp(cmd_line->cmds[0], EXPORT))
        return (myexport(cmd_line, shelly));
    if (!ft_strcmp(cmd_line->cmds[0], PWD))
        return (mypwd(cmd_line, shelly));
    if (!ft_strcmp(cmd_line->cmds[0], UNSET))
        return (mysunset(cmd_line, shelly));
    return (1);
}
