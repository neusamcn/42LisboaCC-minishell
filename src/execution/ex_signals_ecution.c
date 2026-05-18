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

/*
SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\) must be handled diff in:
    - interactive prompt (waiting for input)
    - bltns
    - child process execution
    - waiting for pipelines

A global var used to store the last signal-related exit status: g_signal_stat

sigint_glob():
    When SIGINT is received during execution of bltn commands.
    - ignores the signal parameter
    - sets global status to 130 (standard Ctrl+C exit code)
sigint_prompt_handler (used elsewhere):
    Handles Ctrl+C during interactive prompt (resets line / prints newline)

status_check():
    Converts waitpid() status into anexit code:
    - if process exited normally:
        → return exit status (WEXITSTATUS)
    - if process was terminated by a signal:
        → return 128 + signal number (UNIX convention)
    - if core dump occurred:
        → prints "Quit (core dumped)" message

get_signal_stat():
    Returns the current global signal status.

set_signal_stat():
    Updates the global signal status.

sig_mode(md):
    Changes signal behavior :
    INTERACTIVE:
        - SIGINT handled by custom prompt handler
        - SIGQUIT ignored
    BLT_EXECUTING:
        - SIGINT updates global status (no immediate exit)
        - SIGQUIT ignored
    CHILD:
        - SIGINT and SIGQUIT restored to default behavior
        - allows normal termination of executed programs
    MNDWAIT:
        - SIGINT and SIGQUIT ignored while parent waits for children
*/

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

void	sig_mode(int md)
{
	if (md == INTERACTIVE) // shell waiting for a command
	{
		signal(SIGINT, sigint_prompt_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (md == BLT_EXECUTING) // we are doing a bltn is a parent process
	{
		signal(SIGINT, sigint_glob);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (md == CHILD) // when we are entering child process
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (md == MNDWAIT) // when a parent waits dor a ch process
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}