/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:32:11 by megi              #+#    #+#             */
/*   Updated: 2026/05/24 11:43:44 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

// if no args then PRITN env (instead of returning it)
// checking if the character is valid
// split key=value
// dont exit the shell

int	myexport(t_cmd_line *exp, t_shelly *shelly)
{
	int			i;
	int			f_exported;
	int			oops_error;
	t_export	ex;

	if (exp->cmds[1] == NULL)
	{
		pexp(shelly);
		return (0);
	}
	i = 1;
	oops_error = 0;
	while (exp->cmds[i])
	{
		f_exported = parse_exp_arg(exp->cmds[i]);
		if (f_exported != -1)
			oops_error = 1;
		else
		{
			ex.arg = exp->cmds[i];
			ex.flag = f_exported;
			ex.envp = shelly->envp;
			shelly->envp = exp_flag(&ex);
		}
		i++;
	}
	return (oops_error);
}

int	parse_exp_arg(char *arg)
{
	int		j;
	char	c;

	j = 0;
	while (arg[j])
	{
		c = arg[j];
		if (!exp_argv(c, j))
		{
			mndp_log_err("not valid in this context\n", arg);
			return (-1);
		}
		if (c == '=')
			return (false);
		j++;
	}
	return (true);
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

char	**exp_var(t_export *mini, char *k)
{
	int	i;

	i = 0;
	while (mini->envp[i])
	{
		if (ft_strncmp(mini->envp[i], k, ft_strlen(k)) == 0
			&& mini->envp[i][ft_strlen(k)] == '=')
			return (mini->envp);
		i++;
	}
	return (exp_minienv(mini, k, NULL, -1));
}

char	**exp_minienv(t_export *mini, char *k, char *v, int i)
{
	mini->new_var = ft_strjoin(k, "=");
	if (v)
		mini->new_var = ft_strjoin_free(mini->new_var, v);
	while (mini->envp[++i])
	{
		if (ft_strncmp(mini->envp[i], k, ft_strlen(k)) == 0
			&& mini->envp[i][ft_strlen(k)] == '=')
		{
			free(mini->envp[i]);
			mini->envp[i] = mini->new_var;
			return (mini->envp);
		}
	}
	mini->newenv = malloc(sizeof(char *) * (i + 2));
	if (!mini->newenv)
	{
		print_err_msg("export: malloc failed!");
		return (free(mini->new_var), mini->envp);
	}
	mini->newenv[i] = mini->new_var;
	mini->newenv[i + 1] = NULL;
	while (i--)
		mini->newenv[i] = mini->envp[i];
	return (free(mini->envp), mini->newenv);
}
