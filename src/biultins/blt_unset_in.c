/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_unset_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:24:18 by megi              #+#    #+#             */
/*   Updated: 2026/05/04 17:03:51 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// unset is a builtins that deletes a variable (any var, even the one, you set using an export)

static void	remove_var(t_shelly *shelly, int i)
{
	int	mini_env;

	free(shelly->envp[i]);
	mini_env = i;
	while (shelly->envp[mini_env + 1])
	{
		shelly->envp[mini_env] = shelly->envp[mini_env + 1];
		mini_env++;
	}
	shelly->envp[mini_env] = NULL;
}

int	mysunset(t_cmd_line *unset, t_shelly *shelly)
{
	int		env;
	int		pos;
	int		av;

	av = 1;
	while (unset->cmds[av])
	{
		if (!ft_strchr(unset->cmds[av], '='))
		{
			env = 0;
			while (shelly->envp[env])
			{
				pos = 0;
				while (shelly->envp[env][pos] == unset->cmds[av][pos]
					&& unset->cmds[av][pos])
					pos++;
				if (shelly->envp[env][pos] == '='
					&& !unset->cmds[av][pos])
					return (remove_var(shelly, env), 0);
				env++;
			}
		}
		av++;
	}
	return (0);
}

