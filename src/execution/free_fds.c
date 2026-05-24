/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 17:03:17 by megi              #+#    #+#             */
/*   Updated: 2026/05/24 04:49:37 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_redirs(t_redirects *redir)
{
	t_redirects	*next;

	while (redir)
	{
		next = redir->next;
		if (redir->xd_fd >= 0)
			close(redir->xd_fd);
		free(redir->filename);
		free(redir->delimiter);
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
