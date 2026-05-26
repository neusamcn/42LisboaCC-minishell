/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_utils_in.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 13:48:38 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/25 20:35:57 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

bool	exp_argv(char c, int j)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
		return (VALID);
	if	(j > 0 && c >= MINIM_DIG && c <= MAXIM_DIG) 
		return (VALID);
	if (j > 0 && c == '=')
		return (VALID);
	return (NON_VALID);
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

char	**exp_flag(t_export *exp)
{
	char	*eq;
	char	*k;
	char	*v;

	if (exp->flag == 0)
	{
		eq = ft_strchr(exp->arg, '=');
		k = ft_substr(exp->arg, 0, eq - exp->arg);
		v = eq + 1;
		exp->envp = exp_minienv(exp, k, v, -1);
		free(k);
	}
	else if (exp->flag == 1)
		exp->envp = exp_var(exp, exp->arg);
	return (exp->envp);
}

int	parse_exp_arg(char *arg)
{
	int		j;
	char	c;

	j = 0;
	while (arg[j])
	{
		c = arg[j];
		if (exp_argv(c, j) == NON_VALID)
		{
			mndp_log_err("not valid in this context", arg);
			return (-1);
		}
		if (c == '=')
			return (false);
		j++;
	}
	return (true);
}