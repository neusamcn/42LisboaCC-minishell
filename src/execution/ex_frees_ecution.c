/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_frees_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:09:51 by megi              #+#    #+#             */
/*   Updated: 2026/05/04 16:50:19 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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

void	close_fds(t_redirects redir)
{	
	if (redir.fd[0] != -1)
		close(redir.fd[0]);
	if (redir.fd[1] != -1)
		close(redir.fd[1]);
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

void	cleanup_xd_fds(t_cmd_line *start)
{
	while (start)
	{
		if (start->redir.xd_fd >= 0)
			close(start->redir.xd_fd);
		start = start->next;
	}
}
