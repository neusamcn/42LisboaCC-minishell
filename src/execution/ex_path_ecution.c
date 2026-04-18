/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_path_ecution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 16:27:12 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 01:42:43 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char *relative_path(t_cmd_line *cmd_line, char **envp)
{
    int i;
    char *abs_path;
    
    i = 0;
    abs_path = absolute_path(cmd_line);
    if (abs_path)
        return abs_path;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return paths_helper(cmd_line, envp[i] + 5);
        i++;
    }
    return NULL;
}

char *paths_helper(t_cmd_line *cmd_line, char *path_var)
{
    int j;
    char *abs_path;
    char *tmp;
    char **paths;

    j = 0;
    paths = ft_split(path_var, ':');
    if (!paths)
        return NULL;
    while (paths[j])
    {
        tmp = ft_strjoin(paths[j], "/");
        abs_path = ft_strjoin(tmp, cmd_line->cmds[0]);
        free(tmp);
        if ((access(abs_path, F_OK | X_OK) == 0))
        {
            free_path(paths);
            return abs_path;
        }
        free(abs_path);
        j++;
    }
    free_path(paths);
    return NULL;
    }
    
char *absolute_path(t_cmd_line *cmd_line)
{
    if (ft_strchr(cmd_line->cmds[0], '/'))
    {
        if (access(cmd_line->cmds[0], F_OK | X_OK) == 0)
            return ft_strdup(cmd_line->cmds[0]);
        else
            p_log_err(cmd_line->cmds[0]); // , "No such file or directory\n"
    }
    return 0;
}
