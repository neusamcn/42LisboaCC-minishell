/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellyenvp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 21:16:02 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/03 20:23:36 by ncruz-ne         ###   ########.fr       */
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

char	*find_var_shellyenvp(t_shelly *shelly, char *envp_var_key)
{
	size_t	search_key_len;
	char	*found_envp;
	int		i;

	if (!shelly || !shelly->envp || !envp_var_key)
		return (""); // TODO: exit_cleanup() instead ?
	search_key_len = ft_strlen(envp_var_key);
	i = -1;
	while ((found_envp = shelly->envp[++i]))
	{
		if (!ft_strncmp(found_envp, envp_var_key, search_key_len)
			&& found_envp[search_key_len] == '=')
			return (found_envp + search_key_len + 1);
	}
	return ("");
}

static t_shelly	*set_minimal_shellyenvp(t_shelly *shelly)
{
	char	*cwd;
	char	*path;

	shelly->envp = malloc_protec(sizeof(char *) * 5, shelly);
	path = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	shelly->envp[0] = ft_strdup(path);
	if (!(shelly->envp[0]))
		exit_cleanup(EXIT_FAILURE, shelly);
	cwd = NULL;
	getcwd_protec(cwd, PATH_MAX, shelly);
	// TODO: if result is nil, cwd = "\"?
	shelly->envp[1] = ft_strjoin("PWD=", cwd);
	if (!(shelly->envp[1]))
		exit_cleanup(EXIT_FAILURE, shelly);
	shelly->envp[2] = ft_strdup("SHLVL=1");
	if (!(shelly->envp[2]))
		exit_cleanup(EXIT_FAILURE, shelly);
	shelly->envp[3] = ft_strdup("HOME=");
	if (!(shelly->envp[3]))
		exit_cleanup(EXIT_FAILURE, shelly);
	shelly->envp[4] = NULL;
	// TODO: set PATH, PWD, SHLVL, HOME in hashmap ?
	return (shelly);
}

static char	*eval_set_str_shellyenvp(char **envp, t_shelly *shelly, int i)
{
	int		shlvl_envp;
	char	*shlvl_shellyenvp;

	if (ft_strnstr(envp[i], "SHLVL=", 10))
	{
		shlvl_envp = ft_atoi(ft_strchr(envp[i], '=') + 1);
		if (shlvl_envp < 0)
			shlvl_envp = 0;
		shlvl_shellyenvp = ft_itoa(shlvl_envp + 1);
		if (!shlvl_shellyenvp)
		{
			print_err_msg("ft_itoa() failed");
			exit_cleanup(EXIT_FAILURE, shelly);
		}
		shelly->envp[i] = ft_strjoin("SHLVL=", shlvl_shellyenvp);
		free(shlvl_shellyenvp);
	}
	else
		shelly->envp[i] = ft_strdup(envp[i]);
	return (shelly->envp[i]);
}

static t_shelly	*copy_envp(char **envp, t_shelly *shelly)
{
	size_t		envp_sz;
	size_t		i;

	envp_sz = 0;
	while (envp[envp_sz++])
		;
	shelly->envp = malloc_protec(sizeof(char *) * envp_sz, shelly);
	i = 0;
	while (envp[i])
	{
		shelly->envp[i] = eval_set_str_shellyenvp(envp, shelly, i);
		if (!(shelly->envp[i]))
		{
			print_err_msg("setting shelly_envp failed");
			exit_cleanup(EXIT_FAILURE, shelly);
		}
		i++;
	}
	shelly->envp[i] = NULL;
	return (shelly);
}

t_shelly	*set_shellyenvp(char **envp)
{
	t_shelly	*shelly;

	print_envp_vars(envp); // TODO: delete tester
	shelly = malloc_protec(sizeof(t_shelly), NULL);
	if (!*envp)
		return (set_minimal_shellyenvp(shelly));
	shelly = copy_envp(envp, shelly);
	print_envp_vars(shelly->envp); // TODO: delete tester
	return (shelly);
}
