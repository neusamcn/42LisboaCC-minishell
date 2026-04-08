/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 16:27:12 by megi              #+#    #+#             */
/*   Updated: 2026/04/07 15:55:24 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "execution.h"

// FILE TO PUSH
//TODO: use minienvp instead of envp
//TODO: USE CLOSE_FD TRACKER

char *path(t_pipe *cmd_line, char **envp)
{
    int i;
    char *full_path;

    i = 0;
    full_path = absolute_path(cmd_line);
    if (full_path)
        return full_path;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return paths_helper(cmd_line, envp[i] + 5);
/*         {
            full_path = paths_helper(cmd_line, envp, i);
            return full_path;
        } */
        i++;
    }
    return NULL;
}

char *paths_helper(t_pipe *cmd_line, char *path_var)
{
    int j;
    char *full_path;
    char *tmp;
    char **paths;

    j = 0;
    paths = ft_split(path_var, ':');
    if (!paths)
        return NULL;
    while (paths[j])
    {
        tmp = ft_strjoin(paths[j], "/");
        full_path = ft_strjoin(tmp, cmd_line->cmds[0]);
        free(tmp);
        if ((access(full_path, F_OK | X_OK) == 0))
        {
            free_path(paths);
            return full_path;
        }
        free(full_path);
        j++;
    }
    free_path(paths);
    return NULL;
}

char *absolute_path(t_pipe *cmd_line)
{
    if (ft_strchr(cmd_line->cmds[0], '/'))
    {
        if (access(cmd_line->cmds[0], F_OK | X_OK) == 0)
            return ft_strdup(cmd_line->cmds[0]);
        p_log_err(cmd_line->cmds[0], "No such file or directory\n");
        return NULL;
    }
    return NULL;
}

int free_path(char **paths)
{
    int j;

    j = 0;
    if (!paths)
        return 0;
    while (paths[j])
        free(paths[j++]);
    free(paths);
    return (1);
}
