/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_pwd_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 21:00:13 by megi              #+#    #+#             */
/*   Updated: 2026/05/03 15:52:06 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// searching PWD in the env variables and print 
// ft_strcmp(PWD=) 

int	mypwd(t_cmd_line *cmd, t_minishell *shelly)
{
	int	i;

	(void)cmd;
	i = 0;
	while (shelly->minienvp[i])
	{
		if (ft_strncmp(shelly->minienvp[i], "PWD=", 4) == 0)
		{
			ft_putendl_fd(shelly->minienvp[i] + 4, STDOUT_FILENO);
			return (0);
		}
		i++;
	}
	mndp_log_err("pwd: PWD not setted", cmd->cmds[0]);
	return (1);
}