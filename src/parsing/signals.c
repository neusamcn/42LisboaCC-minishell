/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 21:40:17 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/03 23:04:31 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_sigaction(int signo, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(signo, &sa, NULL) == -1)
	{
		print_err_msg("sigaction failed");
		exit_cleanup(EXIT_FAILURE);
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

void	set_signals_noninteractive(void)
{
	set_sigaction(SIGINT, SIG_DFL, 0);
	set_sigaction(SIGQUIT, SIG_DFL, 0);
}
