/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_utils_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 20:57:40 by megi              #+#    #+#             */
/*   Updated: 2026/05/10 20:17:42 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*abs_or_rel_p(t_cmd_line *c, t_shelly *shelly)
{
	char	*p;

	if (!c || !c->cmds || !c->cmds[0])
		return (NULL);
	p = c->cmds[0];
	if (ft_strchr(p, '/'))
		return (absolute_path(c));
	return (relative_path(c, shelly));
}

bool	if_redir(t_cmd_line *s)
{
	t_redirects	*tmp;

	while (&s->redir)
	{
		if (&s->redir->type != NONE)
			return (true);
		&s->redir = &s->redir->next;
	}
	return (false);
}

void	store_fds(int read_save, int write_save)
{
	dup2(read_save, STDIN_FILENO);
	dup2(write_save, STDOUT_FILENO);
	close(read_save);
	close(write_save);
}
