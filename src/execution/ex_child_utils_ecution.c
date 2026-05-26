/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_child_utils_ecution.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 04:13:46 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 16:55:50 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

void	child_ex_fds(t_cmd_line *kid, t_shelly *shelly)
{
	if (kid->prevfd != -1)
		dup2(kid->prevfd, STDIN_FILENO);
	if (kid->next)
		dup2(kid->pipefd[1], STDOUT_FILENO);
	if (which_redir_type(kid) != 0)
	{
		if (kid->prevfd != -1)
			close(kid->prevfd);
		if (kid->next)
		{
			close(kid->pipefd[0]);
			close(kid->pipefd[1]);
		}
		lonely_child_exit(shelly, NULL, 1);
	}
	if (kid->prevfd != -1)
		close(kid->prevfd);
	if (kid->next)
	{
		close(kid->pipefd[0]);
		close(kid->pipefd[1]);
	}
}

void	lonely_child_exit(t_shelly *shelly, char *path, int status)
{
	free_cmd_line(shelly->cur_cmd);
	babies_cleanup(shelly, NULL);
	free(shelly);
	free(path);
	exit(status);
}

void	babies_cleanup(t_shelly *shelly, char *path)
{
	if (path)
		free(path);
	free_tkn(shelly->cur_tok);
	free_copyenvp(shelly);
}

void	lnly_ch_errmsg(t_cmd_line *kid, t_shelly *shelly)
{
	if (kid->cmds && kid->cmds[0])
	{
		if (errno == EISDIR)
		{
			mndp_log_err("Is a directory", kid->cmds[0]);
			lonely_child_exit(shelly, NULL, 126);
		}
		else
			mndp_log_err("command not found", kid->cmds[0]);
	}
	lonely_child_exit(shelly, NULL, 127);
}

void	child_error(t_cmd_line *kid, t_shelly *shelly)
{
	if (kid->cmds && kid->cmds[0])
		mndp_exec_error(kid->cmds[0]);
	free_cmd_line(shelly->cur_cmd);
	babies_cleanup(shelly, NULL);
	free(shelly);
	exit(127);
}
