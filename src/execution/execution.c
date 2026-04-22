/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/04/22 15:13:25 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	no_cmds_execution(t_cmd_line *cmds, char **envp)
{
    t_redirects *redir;
	int save_out;

	(void)envp;
	if (!cmds->cmds || !cmds->cmds[0])
	{
    	save_out = dup(WRITE);
   		redir = &cmds->redir;
    	while (redir && redir->type != NONE)
    	{
        	if (redir->type == OUT || redir->type == APPEND)
            	append(redir);
			else if (redir->type == IN)
				in_redir(redir);
        	redir = redir->next;
    	}
    	dup2(save_out, WRITE);
    	close(save_out);
    	return ;
	}
}

void exec_loop(t_cmd_line *cmds, char **envp)
{
    t_cmd_line  *tmp;
    t_redirects *redir;

    tmp = cmds;
    while (tmp)
    {
        redir = &tmp->redir;
        while (redir && redir->type != NONE)
        {
            if (redir->type == HEREDOC)
                heredoc(redir);
            redir = redir->next;
        }
        tmp = tmp->next;
    }
	if (!cmds->cmds || !cmds->cmds[0])
		no_cmds_execution(cmds, envp);
    if (cmds->next == NULL && are_you_builtin(cmds) == BUILTINS)
        lonely_blt(cmds, envp);
    else if (cmds->next == NULL)
        mommy_n_father(cmds, envp);
    else
        ex_pipeline_ec(cmds, envp);
}

int lonely_blt(t_cmd_line *s, char **envp)
{
    (void)envp;
    if (if_redir(s))
    {
        s->redir.fd[0] = dup(READ);
        s->redir.fd[1] = dup(WRITE);
        if (s->redir.fd[0] == -1 || s->redir.fd[1] == -1)
            return (perror("dup"), 1);
        if (do_redri(&s->redir) != 0)
        {
            dup2(s->redir.fd[0], READ);
            dup2(s->redir.fd[1], WRITE);
            close(s->redir.fd[0]);
            close(s->redir.fd[1]);
            return (1);
        }
    }
    sig_mode(BLT_EXECUTING);
    // r_bltn(s);
    if (if_redir(s))
    {
        dup2(s->redir.fd[0], READ);
        dup2(s->redir.fd[1], WRITE);
        close(s->redir.fd[0]);
        close(s->redir.fd[1]);
    }
    set_signals_interactive_parent();
    return (get_signal_stat());
}

int mommy_n_father(t_cmd_line *s_cmd, char **envp)
{
	int status;
	pid_t only_child;

	status = 0;
	only_child = fork();
	if (only_child == -1)
		return (perror("fork"), 1);
	if (only_child == 0)
		single_child_ex(s_cmd, envp);
	sig_mode(MNDWAIT);
	waitpid(only_child, &status, 0);
	set_signals_interactive_parent();
	return (status_check(status));
}

int single_child_ex(t_cmd_line *kid, char **envp)
{
	char *path;

    sig_mode(CHILD);
    if (if_redir(kid) && do_redri(&kid->redir) != 0)
        exit(1);
	path = abs_or_rel_p(kid, envp);
	if (!path)
	{
		mndp_log_err("commad not found\n", kid->cmds[0]);
		exit (127);
	}
    execve(path, kid->cmds, envp);
	free (path);
    mndp_log_err("Execution failed!\n", kid->cmds[0]);
    exit(127);
}
