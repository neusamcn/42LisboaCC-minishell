/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 18:08:07 by megiazar          #+#    #+#             */
/*   Updated: 2026/04/24 18:21:37 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

int myenv(t_cmd_line *cmd, char **envp)
{
	size_t i;

	if (cmd->cmds[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (1);
	}
	i = 0;
	while (envp && envp[i])
	{
		ft_putstr_fd(envp[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}