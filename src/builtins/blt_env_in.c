/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 18:08:07 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/25 18:01:58 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

int	myenv(t_cmd_line *cmd, t_shelly *shelly)
{
	size_t	i;

	if (cmd->cmds[1])
	{
		mndp_log_err("no such file or directory", cmd->cmds[1]);
		return (KO);
	}
	i = 0;
	while (shelly->envp && shelly->envp[i])
	{
		if (ft_strchr(shelly->envp[i], '='))
			ft_putendl_fd(shelly->envp[i], 1);
		i++;
	}
	return (OK);
}
