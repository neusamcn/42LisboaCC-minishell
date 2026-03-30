/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 16:27:12 by megi              #+#    #+#             */
/*   Updated: 2026/03/30 20:00:41 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "execution.h"

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
        {
            full_path = paths_helper(cmd_line, envp, i);
            return full_path;
        }
        i++;
    }
    return NULL;
}

char *paths_helper(t_pipe *cmd_line, char **envp, int i)
{
    int j;
    char *full_path;
    char *tmp;
    char **paths;

    j = 0;
    paths = ft_split(envp[i] + 5, ':');
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
        p("minishell: %s: No such file or directory\n", cmd_line->cmds[0]);
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
