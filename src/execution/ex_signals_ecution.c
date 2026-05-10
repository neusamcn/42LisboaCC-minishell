/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   ex_signals_ecution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 16:31:59 by megi              #+#    #+#             */
/*   Updated: 2026/05/10 15:37:56 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

//run oing google.com and then terminate w SIGINT to check the correct way 
static int	g_signal_stat = 0;

void	sigint_glob(int sig)
{
	(void)sig;
	g_signal_stat = 130;
}

int	get_signal_stat(void)
{
	return (g_signal_stat);
}

int	status_check(int status)
{
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = 128 + WTERMSIG(status);
	else if (WCOREDUMP(status))
		ft_putstr_fd("Quit (core dumped)\n", 2);
	return (status);
}

void	set_signal_stat(int value)
{
	g_signal_stat = value;
}

void	sig_mode(int md)
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
