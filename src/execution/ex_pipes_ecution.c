/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_pipes_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/04/29 17:19:41 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static pid_t	fork_pipeline(t_cmd_line *pipeline, t_minishell *shelly)
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

int	ex_pipeline_ec(t_cmd_line *pipeline, t_minishell *shelly)
{
	pid_t		last_st;
	int			status;
	int			cmd_num;
	t_cmd_line	*start;

	start = pipeline;
	pipeline->prevfd = -1;
	last_st = 0;
	cmd_num = 0;
	while (pipeline)
	{
		last_st = fork_pipeline(pipeline, shelly);
		if (last_st == -1)
			return (1);
		cmd_num++;
		pipeline = pipeline->next;
	}
	cleanup_xd_fds(start);
	status = mndwait(last_st, cmd_num);
	return (set_signal_stat(status), 1);
}

int mndwait(pid_t last_p, int cmd_nmb)
{
	pid_t pid;
	int status;
	int last_stat;

	status = 0;
	while (cmd_nmb > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == last_p)
			last_stat = status;
		cmd_nmb--;
	}
	return (status_check(last_stat));
}

void child_ex(char *path, t_cmd_line *kid, t_minishell *shelly)
{
	sig_mode(CHILD);
	if (kid->prevfd != -1)
		dup2(kid->prevfd, 0);
	if (kid->next)
		dup2(kid->pipefd[1], 1);
	if (kid->prevfd != -1)
		close(kid->prevfd);
	if (kid->next)
	{
		close(kid->pipefd[0]);
		close(kid->pipefd[1]);
	}
	if (if_redir(kid))
	{
		if (do_redri(&kid->redir) != 0)
			exit(1);
	}
	if (!kid->cmds || !kid->cmds[0])
		exit(0);
	path = relative_path(kid, shelly);
	if (!path)
	{
		mndp_log_err("Command not found\n", kid->cmds[0]);
		exit(127);
	}
	execve(path, kid->cmds, shelly->minienvp);
	mndp_log_err("Execution failed!\n", kid->cmds[0]);
	exit(127);
}
