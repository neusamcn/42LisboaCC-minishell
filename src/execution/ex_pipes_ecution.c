/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_pipes_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/05/10 20:10:17 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
	1. sets up a fd: stdin is connected to the previous pipe (if it exists), and stdout is connected to the next pipe (if it exists)
	2. hande <, >, >>, + bltn cmds
	3. resolved the cmd path (absolute/relative) by searching the PATH env var
	4. if everything is VALID -> execve() -> replacing the process image
	5. if fail: error printed, child exists with status 127
4) parent processes: loop thru the cmd list :
	1. close unused pipe ends && keep track of process IDs
	2. store the PID of the last cmd (cmd->NULL), because its exist stat defines the final pipeline status, as required by shell behavior 
	3. once all proc. created, parent waits for ALL child proc. && collect the exit status of each proc.
	4. checks if the final status = last command of the pipeline
5) final exit stat is now stored globally
*/

int	ex_pipeline_ec(t_cmd_line *pipeline, t_shelly *shelly)
{
	t_cmd_line	*start;
	pid_t		last_st;
	int			status;
	int			cmd_num;

	start = pipeline;
	pipeline->prevfd = -1;
	last_st = 0;
	cmd_num = 0;
	while (pipeline)
	{
		last_st = fork_pipeline(pipeline, shelly);
		if (last_st == -1)
			break ; // or return 1? 
		cmd_num++;
		pipeline = pipeline->next;
	}
	cleanup_xd_fds(start);
	status = mndwait(last_st, cmd_num);
	return (set_signal_stat(status), 1);
}

static pid_t	fork_pl(t_cmd_line *pipeline, t_shelly *shelly)
{
	pid_t	pid;

	if (pipeline->next)
		pipe(pipeline->pipefd);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
		child_ex(0, pipeline, shelly);
	pipe_cl(pipeline);
	sig_mode(MNDWAIT);
	return (pid);
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
			break ; // or return (1)? 
		if (pid == last_p)
			last_stat = status;
		cmd_nmb--;
	}
	return (status_check(last_stat));
}

void	child_ex_fds(t_cmd_line *kid)
{
	if (kid->prevfd != -1)
		dup2(kid->prevfd, STDIN_FILENO);
	if (kid->next)
		dup2(kid->pipefd[1], STDOUT_FILENO);
	if (kid->prevfd != -1)
		close(kid->prevfd);
	if (kid->next)
	{
		close(kid->pipefd[0]);
		close(kid->pipefd[1]);
	}
	if (which_redir_type(kid) != 0)
		exit(1);
}

void	child_ex(char *path, t_cmd_line *kid, t_shelly *shelly)
{
	sig_mode(CHILD);
	child_ex_fds(kid);
	if (!kid->cmds || !kid->cmds[0])
		exit(0);
	if (are_you_builtin(kid) == false)
	{
		r_bltn(kid, shelly);
		exit(get_signal_stat());
	}
	path = relative_path(kid, shelly);
	if (!path)
	{
		if (kid->cmds && kid->cmds[0])
			exit(mndp_exec_error(kid->cmds[0]));
	}
	execve(path, kid->cmds, shelly->envp);
	if (kid->cmds && kid->cmds[0])
		mndp_log_err("execution failed!\n", kid->cmds[0]);
	exit(127);
}
