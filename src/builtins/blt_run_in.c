/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_run_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 21:00:17 by megi              #+#    #+#             */
/*   Updated: 2026/05/25 19:06:44 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

int	are_you_builtin(t_cmd_line *cmd_line)
{
	if (!cmd_line || !cmd_line->cmds || !cmd_line->cmds[0])
		return (EXTRENAL);
	if ((ft_strcmp(cmd_line->cmds[0], CD) == SHELLYMATES)
		|| (ft_strcmp(cmd_line->cmds[0], ECHO) == SHELLYMATES)
		|| (ft_strcmp(cmd_line->cmds[0], EXIT) == SHELLYMATES)
		|| (ft_strcmp(cmd_line->cmds[0], PWD) == SHELLYMATES)
		|| (ft_strcmp(cmd_line->cmds[0], ENV) == SHELLYMATES)
		|| (ft_strcmp(cmd_line->cmds[0], EXPORT) == SHELLYMATES)
		|| (ft_strcmp(cmd_line->cmds[0], UNSET) == SHELLYMATES))
		return (BUILTINS);
	return (EXTRENAL);
}

int	r_bltn(t_cmd_line *cmd_line, t_shelly *shelly)
{
	if (!cmd_line || !cmd_line->cmds || !cmd_line->cmds[0])
		return (KO);
	if (ft_strcmp(cmd_line->cmds[0], CD) == SHELLYMATES)
		return (mycd(cmd_line, shelly));
	if (ft_strcmp(cmd_line->cmds[0], ECHO) == SHELLYMATES)
		return (myecho(cmd_line, shelly));
	if (ft_strcmp(cmd_line->cmds[0], EXIT) == SHELLYMATES)
		return (myexit(cmd_line, shelly));
	if (ft_strcmp(cmd_line->cmds[0], PWD) == SHELLYMATES)
		return (mypwd(cmd_line, shelly));
	if (ft_strcmp(cmd_line->cmds[0], ENV) == SHELLYMATES)
		return (myenv(cmd_line, shelly));
	if (ft_strcmp(cmd_line->cmds[0], EXPORT) == SHELLYMATES)
		return (myexport(cmd_line, shelly));
	if (ft_strcmp(cmd_line->cmds[0], UNSET) == SHELLYMATES)
		return (mysunset(cmd_line, shelly));
	return (KO);
}
