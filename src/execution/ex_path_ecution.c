/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_path_ecution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 16:27:12 by megi              #+#    #+#             */
/*   Updated: 2026/05/10 18:13:11 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/*Checking if the user pass an absolute path, like <bin/whatever> 
and return it immediately. Only treating str with '/' as paths*/

char	*relative_path(t_cmd_line *cmd_line, t_shelly *shelly)
{
	int		i;
	char	*path;

	i = 0;
	path = absolute_path(cmd_line);
	if (path)
		return (path);
	while (shelly->envp[i])
	{
		if (ft_strncmp(shelly->envp[i], "PATH=", 5) == 0)
			return (paths_helper(cmd_line, shelly->envp[i] + 5));
		i++;
	}
	return (NULL);
}

char	*absolute_path(t_cmd_line *data)
{
	struct		stat sb;
	char		*path;

	if (!data || !data->cmds || !data->cmds[0])
		return (NULL);
	path = data->cmds[0];
	if (!ft_strchr(path, '/'))
		return (NULL);
	if (access(path, F_OK | X_OK) == -1)
		return (NULL);
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		errno = EISDIR;
		return (NULL);
	}
	return (ft_strdup(path));
}

static char	*b_path(char *dir, char *cmd)
{
	char	*path;

	path = ft_strjoin(dir, "/");
	if (!path)
		return (NULL);
	return (ft_strjoin_free(path, cmd));
}

char	*paths_helper(t_cmd_line *cmd_line, char *path_var)
{
	int		j;
	char	*path;
	char	**paths;

	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	j = 0;
	while (paths[j])
	{
		path = b_path(paths[j++], cmd_line->cmds[0]);
		if (path && access(path, F_OK | X_OK) == 0)
			return (free_path(paths), path);
		free(path);
	}
	free_path(paths);
	return (NULL);
}
