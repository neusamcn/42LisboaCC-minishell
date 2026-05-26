/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_exit_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 23:04:46 by megi              #+#    #+#             */
/*   Updated: 2026/05/25 20:57:26 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

// i need to check if the str have only numbers
/*  exit abc      # err - its not a number
exit 1 2      # err - too many aruments  
exit 99999999999999999999  # err - overflow
exit -1       # valid
exit +5       # valid */
/*
exit          exit w last status
exit 42      exit w code 42
exit abc      an err "numeric argument required"
exit 1 2     an err "too many arguments" */
// the formula for the negatibe number will be ((n % 256) + 256) % 256

static int	exit_valid_var(t_cmd_line *arg_line)
{
	int	i;

	i = 0;
	while (arg_line->cmds[1][i] == ' ' || arg_line->cmds[1][i] == '\t' ||
			arg_line->cmds[1][i] == '+' || arg_line->cmds[1][i] == '-')
		i++;
	if ((arg_line->cmds[1][i] < '0' || arg_line->cmds[1][i] > '9'))
		return (0);
	while (arg_line->cmds[1][i] >= '0' && arg_line->cmds[1][i] <= '9')
		i++;
	while (arg_line->cmds[1][i] == ' ' || arg_line->cmds[1][i] == '\t')
		i++;
	return (arg_line->cmds[1][i] == '\0');
}

int	myexit(t_cmd_line *argv, t_shelly *shelly)
{
	sig_mode(INTERACTIVE, shelly);
	if (argv->cmds[1] == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		exit_cleanup(get_signal_stat(), shelly);
	}
	if (argv->cmds[2] != NULL)
	{
		mndp_log_err("bash: exit: too many arguments", argv->cmds[1]);
		return (1);
	}
	if (exit_valid_var(argv) == 1)
	{
		ft_putstr_fd("exit\n", 1);
		exit_cleanup(ft_atoi(argv->cmds[1]) % 256, shelly);
	}
	else
	{
		mndp_log_err("bash: exit %s: needed a numeric arg", argv->cmds[1]);
		return (2);
	}
	return (false);
}
