/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_utils_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 20:57:40 by megi              #+#    #+#             */
/*   Updated: 2026/05/09 16:48:12 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char    *abs_or_rel_p(t_cmd_line *c, t_shelly *shelly)
{
    char *p;
    
    if (!c || !c->cmds || !c->cmds[0])
        return (NULL);
    p = c->cmds[0];
    if (ft_strchr(p, '/'))
        return (absolute_path(c));
    return (relative_path(c, shelly));
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

/* int do_redri(t_redirects *s)
{
	if (s->type == HEREDOC)
	{
		dup2(s->xd_fd, READ);
    	close(s->xd_fd);
	}
	return (which_redir_type(s));
} */

void	store_fds(int read_save, int write_save)
{
	dup2(read_save, 0);
	dup2(write_save, 1);
	close(read_save);
	close(write_save);
}