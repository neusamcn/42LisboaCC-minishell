/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_frees_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:09:51 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 01:03:14 by megi             ###   ########.fr       */
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

void	close_fds(void)
{
	int fd[2];

	if (fd[0] == FD_OPEN)
	{
		close(fd[0]);
		fd[0] = FD_CLOSED;
	}
	if (fd[1] == FD_OPEN)
	{
		close(fd[1]);
		fd[1] = FD_CLOSED;
	}
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
		if (start->redir.xd_fd > 0)
			close(start->redir.xd_fd);
		start = start->next;
	}
}
