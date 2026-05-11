/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 17:03:17 by megi              #+#    #+#             */
/*   Updated: 2026/05/11 18:14:43 by megiazar         ###   ########.fr       */
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

void	cleanup_xd_fds(t_cmd_line *start)
{
	while (start)
	{
		if (start->redir->xd_fd >= 0)
			close(start->redir->xd_fd);
		start = start->next;
	}
}

char *ft_strjoin_free(char *s1, char *s2)
{
    char *res;

    res = ft_strjoin(s1, s2);
    free(s1);
    return res;
}