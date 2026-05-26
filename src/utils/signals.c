/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 21:40:17 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/26 16:17:45 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"
/* 
Note about signal handlers and async-safety:
sigint_prompt_handler calls rl_* and ft_putendl_fd.
Those are not strictly async-signal-safe;
many shells do similar things to integrate with readline,
but the safe alternative is:
in the handler only set a sig_atomic_t flag,
and let the main loop check that flag and call
rl_on_new_line / rl_replace_line / printing from normal code path.
If you see strange crashes, switch to flag-based approach.
*/

volatile sig_atomic_t	g_signal_stat;

static void	sigint_glob(int sig)
{
	(void)sig;
	g_signal_stat = 130;
}

void	set_signal_stat(int value)
{
	g_signal_stat = value;
}

static void	set_sigaction(int signo, void (*handler)(int), int flags, t_shelly *shelly)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(signo, &sa, NULL) == -1)
	{
		print_err_msg("sigaction failed");
		exit_cleanup(EXIT_FAILURE, shelly);
	}
}

void	sig_mode(int md, t_shelly *shelly)
{
	if (md == INTERACTIVE)
	{
		set_sigaction(SIGINT, sigint_prompt_handler, 0, shelly);
		set_sigaction(SIGQUIT, SIG_IGN, 0, shelly);
	}
	else if (md == BLT_EXECUTING)
	{
		set_sigaction(SIGINT, sigint_glob, 0, shelly);
		set_sigaction(SIGQUIT, SIG_IGN, 0, shelly);
	}
	else if (md == CHILD)
	{
		set_sigaction(SIGINT, SIG_DFL, 0, shelly);
		set_sigaction(SIGQUIT, SIG_DFL, 0, shelly);
	}
	else if (md == MNDWAIT)
	{
		set_sigaction(SIGINT, SIG_IGN, 0, shelly);
		set_sigaction(SIGQUIT, SIG_IGN, 0, shelly);
	}
}
