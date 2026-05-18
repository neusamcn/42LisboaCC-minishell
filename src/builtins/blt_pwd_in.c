/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_pwd_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 21:00:13 by megi              #+#    #+#             */
/*   Updated: 2026/05/10 20:21:04 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// searching PWD in the env variables and print 
// ft_strcmp(PWD=) 

int	mypwd(t_cmd_line *cmd, t_shelly *shelly)
{
	int	i;

	(void)cmd;
	i = 0;
	while (shelly->envp[i])
	{
		if (ft_strncmp(shelly->envp[i], "PWD=", 4) == false)
		{
			ft_putendl_fd(shelly->envp[i] + 4, STDOUT_FILENO);
			return (false);
		}
		i++;
	}
	mndp_log_err("pwd: PWD not setted", cmd->cmds[0]);
	return (true);
}
