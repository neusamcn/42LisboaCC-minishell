/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_utils_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 20:57:40 by megi              #+#    #+#             */
/*   Updated: 2026/04/22 15:03:23 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char    *abs_or_rel_p(t_cmd_line *c, char **envp)
{
	char *p;
	
	p = c->cmds[0];
	if (ft_strchr(p, '/'))
		return (absolute_path(c));
	return (relative_path(c, envp));
}

bool if_redir(t_cmd_line *s)
{
    t_redirects *tmp;

    tmp = &s->redir;
    while (tmp)
    {
        if (tmp->type != NONE)
            return (true);
        tmp = tmp->next;
    }
    return (false);
}

/*
bool if_redir(t_cmd_line *s)
{
	return (s->redir.type != NONE);
}*/

int do_redri(t_redirects *s)
{
	if (s->type == HEREDOC)
	{
		dup2(s->xd_fd, READ);
    	close(s->xd_fd);
	}
	return (which_redir_type(s));
}
