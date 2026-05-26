/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_unset_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:24:18 by megi              #+#    #+#             */
/*   Updated: 2026/05/26 20:33:18 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

// unset is a builtins that deletes a variable (any var, even the one, you set 
// using an export)

//TODO: i need to connect nodes to each other

static void	rm(t_shelly *shelly, char *name)
{
	int	env;
	int	i;

	env = 0;
	while (shelly->envp[env])
	{
		i = 0;
		while (shelly->envp[env][i] == name[i] && name[i])
			i++;
		if (shelly->envp[env][ i] == '=' && !name[i])
		{
			free(shelly->envp[env]);
			while (shelly->envp[env])
			{
				shelly->envp[env] = shelly->envp[env + 1];
				env++;
			}
			break ;
		}
		env++;
	}
}

int	mysunset(t_cmd_line *un, t_shelly *shelly)
{
	int	av;

	av = 1;
	while (un->cmds[av])
	{
		if (!ft_strchr(un->cmds[av], '='))
			rm(shelly, un->cmds[av]);
		av++;
	}
	return (OK);
}
