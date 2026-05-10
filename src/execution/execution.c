/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/05/10 19:56:19 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/*
    1) cmds without exec (only redirections)
    2) single command execution (bltb or external)
    3) pipeline execution (multiple cmd)
We are basically deciding which path to follow depending on the cmd list

exec_loop():
    It first processes all HD before execution starts, ensuring that
    all input redirections (<<) are prepared.
    Selects execution mode:
        - no command → only redirections
        - single blt → executed directly in parent
        - single external → forked execution
        - multiple cmds → pipeline exec

no_cmds_execution():
    Only provides redirections:
        - stdout is tmp duplicated && saved && apply redirs
		
single_child_ex():
    Exec a single external cmd inside a child process
        1. switch signal mode to CHILD (default UNIX behavior)
        2. apply redir (<, >, >>, <<)
        3. resolve command path (absolute or PATH search)
        4. if command not found → print error and exit 127
        5. execute program using execve()
        6. if execve fails → print error and exit 127
*/

static void	no_cmds_execution(t_cmd_line *cmds, t_shelly *shelly)
{
	t_redirects	*redir;
	int			save_out;

	(void)shelly->envp;
	if (!cmds->cmds || !cmds->cmds[0])
	{
		save_out = dup(1);
		if (save_out == -1)
			return ;
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

void	exec_loop(t_cmd_line *cmds, t_shelly *shelly)
{
	t_cmd_line	*tmp;
	t_redirects	*redir;

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

int	lonely_blt(t_cmd_line *s, t_shelly *shelly)
{
	int	read_save;
	int	write_save;

	read_save = dup(0);
	write_save = dup(1);
	if (read_save == -1 || write_save == -1)
	{
		if (read_save != -1)
			close(read_save);
		if (write_save != -1)
			close(write_save);
		return (perror("dup"), 1);
	}
	if (if_redir(s) && which_redir_type(s) != 0)
	{
		store_fds(read_save, write_save);
		return (1);
	}
	sig_mode(BLT_EXECUTING);
	r_bltn(s, shelly);
	store_fds(read_save, write_save);
	set_signals_interactive_parent();
	return (get_signal_stat());
}

int	mommy_n_father(t_cmd_line *s_cmd, t_shelly *shelly)
{
	int		status;
	pid_t	only_child;

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

int	single_child_ex(t_cmd_line *kid, t_shelly *shelly)
{
	char	*path;

	sig_mode(CHILD);
	if (which_redir_type(kid) != 0)
		exit(1);
	path = abs_or_rel_p(kid, shelly);
	if (!path)
	{
		if (kid->cmds && kid->cmds[0])
			mndp_log_err("commad not found\n", kid->cmds[0]);
		exit(127);
	}
	execve(path, kid->cmds, shelly->envp);
	free(path);
	mndp_log_err("Execution failed!\n", kid->cmds[0]);
	exit(127);
}
