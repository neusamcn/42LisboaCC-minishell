/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_signals2_ecution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 23:39:55 by megi              #+#    #+#             */
/*   Updated: 2026/04/21 20:59:36 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int status_check(int status)
{
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = 128 + WTERMSIG(status);
	else if (WCOREDUMP(status))
   		ft_putstr_fd("Quit (core dumped)\n", 2); 
	return status;
}

void	sigint_prompt_handler(int signal)
{
	(void)signal;
	ft_putendl_fd("", WRITE);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
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