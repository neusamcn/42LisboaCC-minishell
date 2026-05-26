/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:15:58 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 16:28:46 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

void	redraw_prompt(void)
{
	ft_putendl_fd("", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_prompt_handler(int sig)
{
	(void)sig;
	set_signal_stat(130);
	redraw_prompt();
}

static void	hd_sigint(int sig)
{
	(void)sig;
	set_signal_stat(130);
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	set_heredoc_signals(t_shelly *shelly)
{
	struct sigaction	sa;

	sa.sa_handler = hd_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		print_err_msg("heredoc sigaction failed");
		exit_cleanup(EXIT_FAILURE, shelly);
	}
}
