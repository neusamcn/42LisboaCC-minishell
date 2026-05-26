/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:32:11 by megi              #+#    #+#             */
/*   Updated: 2026/05/26 13:42:12 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

// if no args then PRITN env (instead of returning it)
// checking if the character is valid
// split key=value
// dont exit the shell

int	myexport(t_cmd_line *exp, t_shelly *shelly)
{
	int			i;
	int			f_exported;
	int			oops_error;
	t_export	ex;

	if (exp->cmds[1] == NULL)
		return (pexp(shelly), OK);
	i = 0;
	oops_error = 0;
	while (exp->cmds[++i])
	{
		f_exported = parse_exp_arg(exp->cmds[i]);
		if (f_exported == -1)
			oops_error = 1;
		else
		{
			ex.arg = exp->cmds[i];
			ex.flag = f_exported;
			ex.envp = shelly->envp;
			shelly->envp = exp_flag(&ex);
		}
	}
	return (oops_error);
}
