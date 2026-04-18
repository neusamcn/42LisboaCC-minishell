/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_signals_ecution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 16:31:59 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 01:19:29 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

//run oing google.com and then terminate w SIGINT to check the correct way 
static int g_signal_stat = 0;

void sigint_glob(int sig)
{
    (void)sig;
    g_signal_stat = 130;
}

int get_signal_stat(void)
{
    return g_signal_stat;
}

void set_signal_stat(int value)
{
    g_signal_stat = value;
}

void	set_sigaction(int signo, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(signo, &sa, NULL) == -1)
	{
		print_err_msg("sigaction failed");
		exit(1);
	}
}

void	set_signals_interactive_parent(void)
{
	set_sigaction(SIGINT, sigint_prompt_handler, 0);
	set_sigaction(SIGQUIT, SIG_IGN, 0);
}
