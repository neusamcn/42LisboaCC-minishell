/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_utils_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 20:57:40 by megi              #+#    #+#             */
/*   Updated: 2026/04/16 15:27:15 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	set_signals_interactive_parent(void)
{
	set_sigaction(SIGINT, sigint_prompt_handler, 0);
	set_sigaction(SIGQUIT, SIG_IGN, 0);
}

void sig_mode(int md)
{
	if (md == INTERACTIVE)
	{
		signal(SIGINT, sigint_prompt_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (md == BLT_EXECUTING)
	{
		signal(SIGINT, sigint_glob);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (md == CHILD)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (md == MNDWAIT)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}

int are_you_builtin(t_cmd_line *cmd_line)
{
	if ((!ft_strcmp(cmd_line->cmds[0], CD)) || (!ft_strcmp(cmd_line->cmds[0], ECHO)) || (!ft_strcmp(cmd_line->cmds[0], EXIT)) ||
		(!ft_strcmp(cmd_line->cmds[0], PWD)) || (!ft_strcmp(cmd_line->cmds[0], ENV)) || (!ft_strcmp(cmd_line->cmds[0], EXPORT)) ||
		(!ft_strcmp(cmd_line->cmds[0], UNSET)))
		return (BUILTINS);
	return (EXTRENAL);
}