/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 17:03:17 by megi              #+#    #+#             */
/*   Updated: 2026/05/22 19:33:09 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_fds(t_redirects *redir)
{
	if (!redir)
		return ;
	if (redir->fd[0] != -1)
		close(redir->fd[0]);
	if (redir->fd[1] != -1)
		close(redir->fd[1]);
}

void	free_redirs(t_redirects *redir)
{
	t_redirects	*next;

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

void	cleanup_xd_fds(t_cmd_line *st)
{
	t_redirects	*r;

	while (st)
	{
		r = st->redir;
		while (r)
		{
			if (r->xd_fd >= 0)
				close(r->xd_fd);
			r = r->next;
		}
		st = st->next;
	}
}
