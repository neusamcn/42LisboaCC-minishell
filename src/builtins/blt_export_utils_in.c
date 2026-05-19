/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_utils_in.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 13:48:38 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/19 18:37:02 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"


bool	exp_argv(char c, int j)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| c == '_' || (j > 0 && c >= '0' && c <= '9') || c == '=')
		return (true);
	return (false);
}

void	pexp_var(char *env_entry)
{
	char	*eq;

	eq = ft_strchr(env_entry, '=');
	if (eq)
	{
		write(1, "declare -x ", 11);
		write(1, env_entry, eq - env_entry);
		write(1, "=\"", 2);
		write(1, eq + 1, ft_strlen(eq + 1));
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, "declare -x ", 11);
		write(1, env_entry, ft_strlen(env_entry));
		write(1, "\n", 1);
	}
}

void	pexp(t_shelly *shelly)
{
	int	i;

	i = 0;
	while (shelly->envp[i])
	{
		pexp_var(shelly->envp[i]);
		i++;
	}
}

