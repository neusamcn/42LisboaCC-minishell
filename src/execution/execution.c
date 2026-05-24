/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/05/24 13:18:36 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

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
	int			save_out;
	t_redirects	*r;

	(void)shelly->envp;
	if (!cmds->cmds || !cmds->cmds[0])
	{
		save_out = dup(STDIN_FILENO);
		if (save_out == -1)
			return ;
		r = cmds->redir;
		while (r && r->type != NONE)
		{
			if (r->type == OUT || r->type == APPEND)
				append(r);
			else if (r->type == IN)
				in_redir(r);
			r = r->next;
		}
		dup2(save_out, STDOUT_FILENO);
		close(save_out);
	}
}

void	exec_loop(t_cmd_line *cmds, t_shelly *shelly)
{
	run_xds(cmds, shelly);
	if (!cmds || !cmds->cmds || !cmds->cmds[0] || cmds->cmds[0][0] == '\0')
		no_cmds_execution(cmds, shelly);
	else if (cmds->next == NULL && are_you_builtin(cmds) == BUILTINS)
		g_signal_stat = lonely_blt(cmds, shelly);
	else if (cmds->next == NULL)
		g_signal_stat = mommy_n_father(cmds, shelly);
	else
		ex_pipeline_ec(cmds, shelly);
}

int	lonely_blt(t_cmd_line *s, t_shelly *shelly)
{
	int	read_save;
	int	write_save;
	int	status;

	read_save = dup(STDIN_FILENO);
	write_save = dup(STDOUT_FILENO);
	if (read_save == -1 || write_save == -1)
	{
		if (read_save != -1)
			close(read_save);
		if (write_save != -1)
			close(write_save);
		return (perror("dup"), STDOUT_FILENO);
	}
	if (if_redir(s) && which_redir_type(s) != false)
	{
		store_fds(read_save, write_save);
		return (true);
	}
	sig_mode(BLT_EXECUTING);
	status = r_bltn(s, shelly);
	store_fds(read_save, write_save);
	sig_mode(INTERACTIVE);
	set_signal_stat(status);
	return (status);
}

int	mommy_n_father(t_cmd_line *s_cmd, t_shelly *shelly)
{
	int		status;
	pid_t	only_child;

	status = 0;
	only_child = fork();
	if (only_child == -1)
		return (perror("fork"), 1);
	if (only_child == false)
		single_child_ex(s_cmd, shelly);
	sig_mode(MNDWAIT);
	waitpid(only_child, &status, 0);
	sig_mode(INTERACTIVE);
	return (status_check(status));
}

void	single_child_ex(t_cmd_line *kid, t_shelly *shelly)
{
	char	*path;
	char	**envp;
	char	**argv;

	sig_mode(CHILD);
	if (which_redir_type(kid) != false)
		lonely_child_exit(shelly, NULL, 1);
	path = abs_or_rel_p(kid, shelly);
	if (!path)
		lnly_ch_errmsg(kid, shelly);
	argv = kid->cmds;
	kid->cmds = NULL;
	envp = shelly->envp;
	shelly->envp = NULL;
	free_cmd_line(shelly->cur_cmd);
	babies_cleanup(shelly, NULL);
	free(shelly);
	execve(path, argv, envp);
	free(path);
	mndp_log_err("Execution failed!", argv[0]);
	exit(127);
}
