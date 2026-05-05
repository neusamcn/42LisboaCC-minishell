/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:32:11 by megi              #+#    #+#             */
/*   Updated: 2026/04/30 19:09:58 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// if no args then PRITN env (instead of returning it)
// checking if the character is valid
// split key=value
// dont exit the shell

int	myexport(t_cmd_line *exp, t_minishell *shelly)
{
	int			i;
	int			exported;
	t_export	ex;

	if (exp->cmds[1] == NULL)
	{
		pexp(shelly);
		return (0);
	}
	i = 1;
	while (exp->cmds[i])
	{
		exported = parse_exp_arg(exp->cmds[i]);
		if (exported != -1)
		{
			ex.arg = exp->cmds[i];
			ex.flag = exported;
			ex.envp = shelly->minienvp;
			shelly->minienvp = exp_flag(&ex);
		}
		i++;
	}
	return (0);
}

int parse_exp_arg(char *arg)
{
	int j;
	char c;

	j = 0;
	while (arg[j])
	{
		c = arg[j];
		if (!exp_argv(c, j))
		{
			mndp_log_err("export: not valid in this context\n", arg);
			return (-1);
		}
		if (c == '=')
			return (0);
		j++;
	}
	return (1);
}

char **exp_flag(t_export *exp)
{
	char **split;
	int i;

    i = 0;
	if (exp->flag == 0)
	{
		split = ft_split(exp->arg, '=');
		if (split)
		{
			exp->envp = exp_minienv(exp, split[0], split[1], -1);
			while (split[i])
				free(split[i++]);
			free(split);
		}
	}
	else if (exp->flag == 1)
		exp->envp = exp_var(exp, exp->arg);
	return (exp->envp);
}

char **exp_var(t_export *mini, char *key)
{
	int i;

	i = 0;
	while (mini->envp[i])
	{
		if (ft_strncmp(mini->envp[i], key, ft_strlen(key)) == 0 && 
			mini->envp[i][ft_strlen(key)] == '=')
			return (mini->envp);
		i++;
	}
	return (exp_minienv(mini, key, NULL, -1));
}

char	**exp_minienv(t_export *mini, char *key, char *value, int i)
{
	mini->new_var = ft_strjoin(key, "=");
	if (value)
		mini->new_var = ft_free_strjoin(mini->new_var, value);
	while (mini->envp[++i])
	{
		if (ft_strncmp(mini->envp[i], key, ft_strlen(key)) == 0 && 
			mini->envp[i][ft_strlen(key)] == '=')
		{
			free(mini->envp[i]);
			mini->envp[i] = mini->new_var;
			return (mini->envp);
		}
	}
	mini->newenv = malloc(sizeof(char *) * (i + 2));
	if (!mini->newenv)
		return (free(mini->new_var), mini->envp);
	mini->newenv[i] = mini->new_var;
	mini->newenv[i + 1] = NULL;
	while (i--)
		mini->newenv[i] = mini->envp[i];
	return (free(mini->envp), mini->newenv);
}
