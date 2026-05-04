/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_unset_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:24:18 by megi              #+#    #+#             */
/*   Updated: 2026/05/04 23:48:12 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// unset is a builtins that deletes a variable (any var, even the one, you set using an export)

static void	rm(t_shelly *shelly, char *name)
{
	int	env;
	int	pos;

	env = 0;
	while (shelly->envp[env])
	{
		pos = 0;
		while (shelly->envp[env][pos] == name[pos] && name[pos])
			pos++;
		if (shelly->envp[env][pos] == '=' && !name[pos])
		{
			remove_var(shelly, env);
			break ;
		}
		env++;
	}
}

int	mysunset(t_cmd_line *unset, t_shelly *shelly)
{
	int	av;

	av = 1;
	while (unset->cmds[av])
	{
		if (!ft_strchr(unset->cmds[av], '='))
			find_and_remove(shelly, unset->cmds[av]);
		av++;
	}
	return (0);
}

