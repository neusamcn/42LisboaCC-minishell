/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_signals_ecution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 16:31:59 by megi              #+#    #+#             */
/*   Updated: 2026/04/15 21:05:59 by megi             ###   ########.fr       */
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

static void	set_sigaction(int signo, void (*handler)(int), int flags)
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

void	sigint_prompt_handler(int signal)
{
	(void)signal;
	ft_putendl_fd("", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_signals_interactive_parent(void)
{
	set_sigaction(SIGINT, sigint_prompt_handler, 0);
	set_sigaction(SIGQUIT, SIG_IGN, 0);
}

int status_check(int status)
{
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = 128 + WTERMSIG(status);
	return status;
}
