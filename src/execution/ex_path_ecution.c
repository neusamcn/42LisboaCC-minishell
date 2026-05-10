/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_path_ecution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 16:27:12 by megi              #+#    #+#             */
/*   Updated: 2026/05/09 20:51:19 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	cmd_not_found(char *cmd, char *err)
{
	char	*msg;
	char	*tmp;

	tmp = ft_strjoin("minishell: ", cmd);
	if (!tmp)
		return ;
	msg = ft_strjoin(tmp, err);
	free(tmp);
	if (!msg)
		return ;
	ft_putstr_fd(msg, 2);
	free(msg);
}

char *relative_path(t_cmd_line *cmd_line, t_shelly *shelly)
{
    int i;
    char *path;
    
    i = 0;
    path = absolute_path(cmd_line);
    if (path)
        return path;
    while (shelly->envp[i])
    {
        if (ft_strncmp(shelly->envp[i], "PATH=", 5) == 0)
            return paths_helper(cmd_line, shelly->envp[i] + 5);
        i++;
    }
    return NULL;
}

char *absolute_path(t_cmd_line *data)
{
    struct stat sb;

    if (!data || !data->cmds || !data->cmds[0])
        return (NULL);
    if (!ft_strchr(data->cmds[0], '/'))
        return (NULL);
    if (stat(data->cmds[0], &sb) == -1)
    {
        if (errno == EACCES)
            cmd_not_found(data->cmds[0], ": Permission denied\n");
        else
            cmd_not_found(data->cmds[0], ": No such file or directory\n");
        return (NULL);
    }
    if (S_ISDIR(sb.st_mode))
    {
        cmd_not_found(data->cmds[0], ": Is a directory\n");
        return (NULL);
    }
    if (access(data->cmds[0], X_OK) == -1)
    {
        cmd_not_found(data->cmds[0], ": Permission denied\n");
        return (NULL);
    }
    return (ft_strdup(data->cmds[0]));
}

static char *b_path(char *dir, char *cmd)
{
    char *path;

    path = ft_strjoin(dir, "/");
    if (!path)
        return (NULL);
    return (ft_strjoin_free(path, cmd));
}

char *paths_helper(t_cmd_line *cmd_line, char *path_var)
{
    int     j;
    char    *path;
    char    **paths;

    if (ft_strcmp(cmd_line->cmds[0], ".") == 0
        || ft_strcmp(cmd_line->cmds[0], "..") == 0)
        return (cmd_not_found(cmd_line->cmds[0], ": command not found\n"), NULL);
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
