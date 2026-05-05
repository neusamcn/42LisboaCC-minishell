/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_pipes_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/05/03 20:28:23 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h" // Milena, I corrected your path

static pid_t	fork_pipeline(t_cmd_line *pipeline, t_shelly *shelly)
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

int	ex_pipeline_ec(t_cmd_line *pipeline, t_shelly *shelly)
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
	if (if_redir(kid) && do_redri(&kid->redir) != 0)
		exit(1);
}

void	child_ex(char *path, t_cmd_line *kid, t_shelly *shelly)
{
	sig_mode(CHILD);
	child_ex_fds(kid);
	if (!kid->cmds || !kid->cmds[0])
		exit(0);
	if (are_you_builtin(kid) == 0)
	{
		r_bltn(kid, shelly);
		exit(get_signal_stat());
	}
	path = relative_path(kid, shelly);
	if (!path)
	{
		mndp_log_err("command not found\n", kid->cmds[0]);
		exit(127);
	}
	execve(path, kid->cmds, shelly->envp);
	mndp_log_err("execution failed!\n", kid->cmds[0]);
	exit(127);
}

