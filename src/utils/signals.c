/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 21:40:17 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/18 19:43:41 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//run oing google.com and then terminate w SIGINT to check the correct way 
static int g_signal_stat = 0;

static void	sigint_glob(int sig)
{
	(void)sig;
	g_signal_stat = 130;
}

void	set_signal_stat(int value)
{
	g_signal_stat = value;
}

// TODO: add *minishell to args?
static void	set_sigaction(int signo, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(signo, &sa, NULL) == -1)
	{
		print_err_msg("sigaction failed");
		exit_cleanup(EXIT_FAILURE, NULL); // TODO: review adding *minishell
	}
}

static void	sigint_prompt_handler(int signal)
{
	(void)signal;
	set_signal_stat(130);
	ft_putendl_fd("", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// // TODO: replace with sig_mode()?
// void	set_signals_interactive_parent(void)
// {
// 	set_sigaction(SIGINT, sigint_prompt_handler, 0);
// 	set_sigaction(SIGQUIT, SIG_IGN, 0);
// }

// // TODO: replace with sig_mode()?
// void	set_signals_noninteractive(void)
// {
// 	set_sigaction(SIGINT, SIG_DFL, 0);
// 	set_sigaction(SIGQUIT, SIG_DFL, 0);
// }

void	sig_mode(int md)
{
	if (md == INTERACTIVE) // shell waiting for a command
	{
		set_sigaction(SIGINT, sigint_prompt_handler, 0);
		set_sigaction(SIGQUIT, SIG_IGN, 0);
	}
	else if (md == BLT_EXECUTING) // we are doing a bltn is a parent process
	{
		set_sigaction(SIGINT, sigint_glob, 0);
		set_sigaction(SIGQUIT, SIG_IGN, 0);
	}
	else if (md == CHILD) // when we are entering child process
	{
		set_sigaction(SIGINT, SIG_DFL, 0);
		set_sigaction(SIGQUIT, SIG_DFL, 0);
	}
	else if (md == MNDWAIT) // when a parent waits dor a ch process
	{
		set_sigaction(SIGINT, SIG_IGN, 0);
		set_sigaction(SIGQUIT, SIG_IGN, 0);
	}
}
