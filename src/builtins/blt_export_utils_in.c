/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_utils_in.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 13:48:38 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 18:20:37 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

bool	exp_argv(char c, int j)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
		return (VALID);
	if (j > 0 && c >= MINIM_DIG && c <= MAXIM_DIG)
		return (VALID);
	if (j > 0 && c == '=')
		return (VALID);
	return (NON_VALID);
}

void	pexp(t_shelly *shelly)
{
	int		i;
	char	*eq_symb;

	i = -1;
	while (shelly->envp[++i])
	{
		eq_symb = ft_strchr(shelly->envp[i], '=');
		if (eq_symb)
		{
			write(1, "declare -x ", 11);
			write(1, shelly->envp[i], eq_symb - shelly->envp[i]);
			write(1, "=\"", 2);
			write(1, eq_symb + 1, ft_strlen(eq_symb + 1));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, "declare -x ", 11);
			write(1, shelly->envp[i], ft_strlen(shelly->envp[i]));
			write(1, "\n", 1);
		}
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
