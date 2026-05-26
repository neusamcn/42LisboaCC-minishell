/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_exit_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 23:04:46 by megi              #+#    #+#             */
/*   Updated: 2026/05/26 10:38:48 by megiazar         ###   ########.fr       */
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

static int	valid_or_not(t_cmd_line *arg_line)
{
	int		i;
	char	*arg;

	i = 0;
	arg = arg_line->cmds[1];
	while (arg[i] == ' ' || arg[i] == TAB || ft_strchr(PLUS_OR_MINUS, arg[i]))
		i++;
	if (arg[i] < MINIM_DIG || arg[i] > MAXIM_DIG)
		return (NON_VALID);
	while (arg[i] >= MINIM_DIG && arg[i] <= MAXIM_DIG)
		i++;
	while (ft_strchr(WHITESPACE, arg[i]))
		i++;
	if (arg[i] == '\0')
		return (VALID);
	return (NON_VALID);
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
		mndp_log_err("too many arguments", argv->cmds[1]);
		return (KO);
	}
	if (valid_or_not(argv) == VALID)
	{
		ft_putstr_fd("exit\n", 1);
		exit_cleanup(ft_atoi(argv->cmds[1]) % 256, shelly);
	}
	else
	{
		mndp_log_err("numeric argumedn requiered", argv->cmds[1]);
		return (2);
	}
	return (OK);
}
