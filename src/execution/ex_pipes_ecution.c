/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_pipes_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/05/24 20:39:25 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

/*
This file executes a parsed cmd line (ls | grep a | wc -l) by turning it 
into a pipeline of processes. We are using a linked list (cmd1 → cmd2 → cmd3) 
&& executeed processes where each connected thru a pipe w next one
Every command becomes an independent process created with fork().
These processes are connected so that the stdout of one process becomes the 
stdin of the next one, allowing data to flow through the pipeline.

The execution starts by going through the list of cmds:
1) checking if there is a next command -> creates a pipe
2) fork && split execution into parent and child processes
3) child process: execut single cmd :
	1. sets up a fd: stdin is connected to the previous pipe (if it exists), 
	and stdout is connected to the next pipe (if it exists)
	2. hande <, >, >>, + bltn cmds
	3. resolved the cmd path (absolute/relative) by searching the PATH env var
	4. if everything is VALID -> execve() -> replacing the process image
	5. if fail: error printed, child exists with status 127
4) parent processes: loop thru the cmd list :
	1. close unused pipe ends && keep track of process IDs
	2. store the PID of the last cmd (cmd->NULL), cause its exist stat defines 
	the final pipeline status, as required by shell behavior 
	3. once all proc. created, parent waits for ALL child proc. && collect the
	exit status of each proc.
	4. checks if the final status = last command of the pipeline
5) final exit stat is now stored globally
*/

static pid_t	fork_pl(t_cmd_line *pl, t_shelly *shelly)
{
	pid_t	pid;

	if (pl->next)
		pipe(pl->pipefd);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
		child_ex(0, pl, shelly);
	if (pl->next)
		close(pl->pipefd[1]);
	sig_mode(MNDWAIT);
	return (pid);
}

int	ex_pipeline_ec(t_cmd_line *pl, t_shelly *shelly)
{
	int			status;
	int			cmd_num;
	pid_t		last_stat;
	t_cmd_line	*st;

	st = pl;
	pl->prevfd = -1;
	last_stat = 0;
	cmd_num = 0;
	while (pl)
	{
		last_stat = fork_pl(pl, shelly);
		if (last_stat == -1)
			break ;
		if (pl->prevfd != -1)
			close(pl->prevfd);
		if (pl->next)
			pl->next->prevfd = pl->pipefd[0];
		cmd_num++;
		pl = pl->next;
	}
	cleanup_xd_fds(st);
	status = mndwait(last_stat, cmd_num);
	return (set_signal_stat(status), 1);
}

int	mndwait(pid_t last_p, int cmd_nmb)
{
	pid_t	pid;
	int		status;
	int		last_stat;

	status = 0;
	while (cmd_nmb > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (pid == last_p)
			last_stat = status;
		cmd_nmb--;
	}
	return (status_check(last_stat));
}

static void	child_ex_execve(t_cmd_line *kid, t_shelly *shelly)
{
	char	**envp;
	char	**argv;
	char	*path;

	path = relative_path(kid, shelly);
	if (!path)
	{
		if (kid->cmds && kid->cmds[0])
			mndp_exec_error(kid->cmds[0]);
		free_cmd_line(shelly->cur_cmd);
		babies_cleanup(shelly, NULL);
		free(shelly);
		exit(127);
	}
	argv = kid->cmds;
	kid->cmds = NULL;
	envp = shelly->envp;
	shelly->envp = NULL;
	free_cmd_line(shelly->cur_cmd);
	babies_cleanup(shelly, NULL);
	free(shelly);
	execve(path, argv, envp);
	mndp_log_err("execution failed!\n", argv[0]);
	exit(127);
}

void	child_ex(char *path, t_cmd_line *kid, t_shelly *shelly)
{
	sig_mode(CHILD);
	path = NULL;
	(void)path; // Milena, I added this to be able to compile
	child_ex_fds(kid, shelly);
	if (!kid->cmds || !kid->cmds[0])
	{
		free_cmd_line(shelly->cur_cmd);
		babies_cleanup(shelly, NULL);
		free(shelly);
		exit(0);
	}
	if (are_you_builtin(kid) == BUILTINS)
	{
		r_bltn(kid, shelly);
		free_cmd_line(shelly->cur_cmd);
		babies_cleanup(shelly, NULL);
		free(shelly);
		exit(get_signal_stat());
	}
	child_ex_execve(kid, shelly);
}
