/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_run_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 21:00:17 by megi              #+#    #+#             */
/*   Updated: 2026/04/29 14:09:10 by megiazar         ###   ########.fr       */
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

int r_bltn(t_cmd_line *cmd_line, char **envp)
{
    if (!cmd_line || !cmd_line->cmds || !cmd_line->cmds[0])
        return (1);
    if (!ft_strcmp(cmd_line->cmds[0], ECHO))
        return (myecho(cmd_line, envp));
    if (!ft_strcmp(cmd_line->cmds[0], ENV))
        return (myenv(cmd_line, envp));
    if (!ft_strcmp(cmd_line->cmds[0], EXPORT))
        return (myexport(cmd_line, envp));
    return (1);
}