/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienvp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 21:16:02 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/29 18:37:14 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// TODO: remove tester:
static void	print_envp_vars(char **envp)
{
	int	i = 0;
	while (envp[i])
	{
		ft_printf(MAGENTA"%d"COLOR_RESET": %s\n\n", i, envp[i]);
		i++;
	}
}

static t_minishell	*set_minimal_minienvp(t_minishell *minishell)
{
	char	*cwd;
	char	*path;

	minishell->minienvp = malloc_protec(sizeof(char *) * 5, minishell);
	path = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	minishell->minienvp[0] = ft_strdup(path);
	if (!(minishell->minienvp[0]))
		exit_cleanup(EXIT_FAILURE, minishell);
	cwd = NULL;
	getcwd_protec(cwd, PATH_MAX, minishell);
	// TODO: if result is nil, cwd = "\"?
	minishell->minienvp[1] = ft_strjoin("PWD=", cwd);
	if (!(minishell->minienvp[1]))
		exit_cleanup(EXIT_FAILURE, minishell);
	minishell->minienvp[2] = ft_strdup("SHLVL=1");
	if (!(minishell->minienvp[2]))
		exit_cleanup(EXIT_FAILURE, minishell);
	minishell->minienvp[3] = ft_strdup("HOME=");
	if (!(minishell->minienvp[3]))
		exit_cleanup(EXIT_FAILURE, minishell);
	minishell->minienvp[4] = NULL;
	// TODO: set PATH, PWD, SHLVL, HOME in hashmap ?
	return (minishell);
}

static char	*eval_set_strminienvp(char **envp, t_minishell *minishell, int i)
{
	int		shlvl_envp;
	char	*shlvl_minienvp;

	if (ft_strnstr(envp[i], "SHLVL=", 10))
	{
		shlvl_envp = ft_atoi(ft_strchr(envp[i], '=') + 1);
		if (shlvl_envp < 0)
			shlvl_envp = 0;
		shlvl_minienvp = ft_itoa(shlvl_envp + 1);
		if (!shlvl_minienvp)
		{
			print_err_msg("ft_itoa() failed");
			exit_cleanup(EXIT_FAILURE, minishell);
		}
		minishell->minienvp[i] = ft_strjoin("SHLVL=", shlvl_minienvp);
		free(shlvl_minienvp);
	}
	else
		minishell->minienvp[i] = ft_strdup(envp[i]);
	return (minishell->minienvp[i]);
}

t_minishell	*copy_envp(char **envp, t_minishell *minishell)
{
	size_t		envp_sz;
	size_t		i;

	envp_sz = 0;
	while (envp[envp_sz++])
		;
	minishell->minienvp = malloc_protec(sizeof(char *) * envp_sz, minishell);
	i = 0;
	while (envp[i])
	{
		minishell->minienvp[i] = eval_set_strminienvp(envp, minishell, i);
		if (!(minishell->minienvp[i]))
		{
			print_err_msg("setting minienvp failed");
			exit_cleanup(EXIT_FAILURE, minishell);
		}
		i++;
	}
	minishell->minienvp[i] = NULL;
	return (minishell);
}

t_minishell	*set_minienvp(char **envp)
{
	t_minishell	*minishell;

	print_envp_vars(envp);
	minishell = malloc_protec(sizeof(t_minishell), NULL);
	if (!*envp)
		return (set_minimal_minienvp(minishell));
	minishell = copy_envp(envp, minishell);
	print_envp_vars(minishell->minienvp);
	return (minishell);
}
