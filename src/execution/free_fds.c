/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 17:03:17 by megi              #+#    #+#             */
/*   Updated: 2026/05/12 11:36:34 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	cleanup_xd_fds(t_cmd_line *start)
{
	while (start)
	{
		if (start->redir->xd_fd >= 0)
			close(start->redir->xd_fd);
		start = start->next;
	}
}
