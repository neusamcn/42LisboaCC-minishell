/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_frees_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:09:51 by megi              #+#    #+#             */
/*   Updated: 2026/05/09 17:11:15 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void    ft_free_split(char **arr)
{
    int i;

    if (!arr)
        return ;
    i = 0;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}

void    free_redirs(t_redirects *redir)
{
    t_redirects *next;

    if (!redir)
        return ;
    if (redir->xd_fd >= 0)
        close(redir->xd_fd);
    redir = redir->next;
    while (redir)
    {
        next = redir->next;
        if (redir->xd_fd >= 0)
            close(redir->xd_fd);
        free(redir);
        redir = next;
    }
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
    return (true);
}

void pipe_cl(t_cmd_line *pipeline)
{
    if (pipeline->prevfd != -1)
        close(pipeline->prevfd);
    if (pipeline->next)
    {
        close(pipeline->pipefd[1]);
        pipeline->next->prevfd = pipeline->pipefd[0];
    }
}

void    free_cmd_line(t_cmd_line *cmd)
{
    t_cmd_line  *next;

    if (!cmd)
        return ;
    while (cmd)
    {
        next = cmd->next;
        ft_free_split(cmd->cmds);
        free_redirs(&cmd->redir);
        free(cmd);
        cmd = next;
    }
}