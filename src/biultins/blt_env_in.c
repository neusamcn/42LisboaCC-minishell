/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 18:08:07 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/06 15:53:19 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

int myenv(t_cmd_line *cmd, t_shelly *shelly)
{
	size_t i;

	if (cmd->cmds[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (true);
	}
	i = 0;
	while (shelly->envp && shelly->envp[i])
	{
		if (ft_strchr(shelly->envp[i], '='))
			ft_putendl_fd(shelly->envp[i], 1);
		i++;
	}
	return (false);
}
