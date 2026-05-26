/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_utils2_in.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:27:41 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 18:23:56 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

char	*val_creates(char *k, char *v)
{
	char	*s;

	s = ft_strjoin(k, "=");
	if (v)
		s = ft_strjoin_free(s, v);
	return (s);
}

int	find_var_inside_env(char **envp, char *k)
{
	int		i;

	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], k, ft_strlen(k)) == SHELLYPEERS
			&& envp[i][ft_strlen(k)] == '=')
			return (i);
	return (-1);
}

char	**add_newvar(char **envp, char *s, int sz)
{
	char	**newenvp;

	newenvp = malloc(sizeof(char *) * (sz + 2));
	if (!newenvp)
		return (free(s), envp);
	newenvp[sz] = s;
	newenvp[sz + 1] = NULL;
	while (sz--)
		newenvp[sz] = envp[sz];
	return (free(envp), newenvp);
}

char	**exp_minienv(t_export *mini, char *k, char *v, int sz)
{
	char	*s;
	int		pos;

	s = val_creates(k, v);
	pos = find_var_inside_env(mini->envp, k);
	if (pos != -1)
	{
		free(mini->envp[pos]);
		mini->envp[pos] = s;
		return (mini->envp);
	}
	if (sz == -1)
	{
		sz = 0;
		while (mini->envp[sz])
			sz++;
	}
	return (add_newvar(mini->envp, s, sz));
}

char	**exp_var(t_export *mini, char *k)
{
	if (find_var_inside_env(mini->envp, k) != -1)
		return (mini->envp);
	return (exp_minienv(mini, k, NULL, -1));
}
