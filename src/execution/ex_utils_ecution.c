/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_utils_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 20:57:40 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 01:12:37 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char    *abs_or_rel_p(t_cmd_line *c, char **envp)
{
	char *p;
	
	p = c->cmds[0];
	if (!p)
	{
		p_log_err("Command not found\n");
		return (NULL);
	}
	if (ft_strchr(p, '/'))
		return (absolute_path(c));
	return(relative_path(c, envp));
}

bool if_redir(t_cmd_line *s)
{
	return (s->redir.type != NONE);
}

void do_redri(t_cmd_line *s)
{
	pipe_handler(&s->redir);
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