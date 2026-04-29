/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/04/29 17:16:48 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void no_cmds_execution(t_cmd_line *cmds, t_minishell *shelly)
{
	t_redirects *redir;
	int save_out;

	(void)shelly->minienvp;
	if (!cmds->cmds || !cmds->cmds[0])
	{
		save_out = dup(1);
		if (save_out == -1)
			return;
		redir = &cmds->redir;
		while (redir && redir->type != NONE)
		{
			if (redir->type == OUT || redir->type == APPEND)
				append(redir);
			else if (redir->type == IN)
				in_redir(redir);
			redir = redir->next;
		}
		dup2(save_out, 1);
		close(save_out);
	}
}

void exec_loop(t_cmd_line *cmds, t_minishell *shelly)
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
		no_cmds_execution(cmds, shelly);
	if (cmds->next == NULL && are_you_builtin(cmds) == BUILTINS)
		lonely_blt(cmds, shelly);
	else if (cmds->next == NULL)
		mommy_n_father(cmds, shelly);
	else
		ex_pipeline_ec(cmds, shelly);
}

int lonely_blt(t_cmd_line *s, t_minishell *shelly)
{
    int read_save;
    int write_save;

    (void)shelly->minienvp;
    read_save = dup(0);
    write_save = dup(1);
    if (read_save == -1 || write_save == -1)
        return (perror("dup"), 1);
    if (if_redir(s))
    {
        if (do_redri(&s->redir) != 0)
        {
            dup2(read_save, 0);
            dup2(write_save, 1);
            close(read_save);
            close(write_save);
            return (1);
        }
    }
    sig_mode(BLT_EXECUTING);
    r_bltn(s, shelly);
    dup2(read_save, 0);
    dup2(write_save, 1);
    close(read_save);
    close(write_save);
    set_signals_interactive_parent();
    return (get_signal_stat());
}

int mommy_n_father(t_cmd_line *s_cmd, t_minishell *shelly)
{
	int status;
	pid_t only_child;

	status = 0;
	only_child = fork();
	if (only_child == -1)
		return (perror("fork"), 1);
	if (only_child == 0)
		single_child_ex(s_cmd, shelly);
	sig_mode(MNDWAIT);
	waitpid(only_child, &status, 0);
	set_signals_interactive_parent();
	return (status_check(status));
}

int single_child_ex(t_cmd_line *kid, t_minishell *shelly)
{
	char *path;

	sig_mode(CHILD);
	if (if_redir(kid) && do_redri(&kid->redir) != 0)
		exit(1);
	path = abs_or_rel_p(kid, shelly);
	if (!path)
	{
		mndp_log_err("commad not found\n", kid->cmds[0]);
		exit (127);
	}
	execve(path, kid->cmds, shelly->minienvp);
	free (path);
	mndp_log_err("Execution failed!\n", kid->cmds[0]);
	exit(127);
}
