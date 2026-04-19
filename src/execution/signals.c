/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 16:31:59 by megi              #+#    #+#             */
/*   Updated: 2026/04/08 16:17:41 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"


// FILE TO PUSH IN MAIN BRUNCH

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
		exit_cleanup(EXIT_FAILURE, NULL);
	}
}

static void	sigint_prompt_handler(int signal)
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

void	set_signals_noninteractive(void)
{
	set_sigaction(SIGINT, SIG_DFL, 0);
	set_sigaction(SIGQUIT, SIG_DFL, 0);
}