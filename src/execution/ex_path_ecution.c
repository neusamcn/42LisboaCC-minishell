/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_path_ecution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 16:27:12 by megi              #+#    #+#             */
/*   Updated: 2026/05/24 13:26:40 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

/*
Searching if the cmd is an absolute or relative path or by using the PATH
envp var. ( ls to /bin/ls )
1. absolute path check:
    If the cmd contains a '/',-> direct path.
		1) access: if the file exists and is executable
        2) stat: checks if it is a directory
        3) if it is a directory → errno is set to EISDIR
        4) if valid → a duplicated copy of the path is returned
2. PATH search:
    If the cmd is not an absolute path, the PATH envp var is searched.
    The PATH string is split by ':' into directories.
    Each directory is combined with the cmd name to form a candidate path 
	(dir + "/" + cmd).
    For each candidate:
        1) access: checks if the file exists and is executable
        2) if valid → the full path is returned immediately
        3) otherwise it is freed and the next path is tested
*/

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
	char		*path;
	struct stat	sb;

	if (!data || !data->cmds || !data->cmds[0])
		return (NULL);
	path = data->cmds[0];
	if (!ft_strchr(path, '/'))
		return (NULL);
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		errno = EISDIR;
		return (NULL);
	}
	if (access(path, F_OK | X_OK) == -1)
		return (NULL);
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
