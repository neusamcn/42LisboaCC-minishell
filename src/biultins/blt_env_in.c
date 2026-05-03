/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 18:08:07 by megiazar          #+#    #+#             */
/*   Updated: 2026/04/29 16:58:37 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

int myenv(t_cmd_line *cmd, t_minishell *shelly)
{
	size_t i;

	if (cmd->cmds[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (1);
	}
	i = 0;
	while (shelly->minienvp && shelly->minienvp[i])
	{
		if (ft_strchr(shelly->minienvp[i], '='))
			ft_putendl_fd(shelly->minienvp[i], 1);
		i++;
	}
	return (0);
}
