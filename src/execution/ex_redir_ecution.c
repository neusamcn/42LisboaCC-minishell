/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_redir_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 23:41:36 by megi              #+#    #+#             */
/*   Updated: 2026/05/25 19:41:00 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

/*
Input/output redirections && HD for a single cmd in a pipeline
Redirections modify how a cmd reads input and writes output by changing
its file descriptors before execution.
A cmd may have:
		NONE, // 0
		IN,  // < redir input to a cmd, taking input from a file
		OUT, // > redir output to a file & overwr the file if it alreexists
		APPEND, // >> r output top a file, app the output to the end of the f
		HEREDOC, // <<
They are applied BEFORE execve in child process.

HEREDOC processes;
    Baby:
        - Reads user input line by line using rl
        - Stops when delimiter is found
        - Writes input into pipe write-end
        - If EOF is reached -> warning message
    Mom and dad:
        - Waits for child proc. to finish
        - Restores signal mode
        - Duplicates pipe read-end into redirection storage (xd_fd)
        - This fd will later be used as stdin during execution
*/

bool	which_redir_type(t_cmd_line *cmd)
{
	t_redirects	*redir;

	if (!cmd)
		return (false);
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (redir->xd_fd == -1)
				return (KO);
			dup2(redir->xd_fd, STDIN_FILENO);
			close(redir->xd_fd);
		}
		else if (redir->type == APPEND || redir->type == OUT)
		{
			if (append(redir))
				return (KO);
		}
		else if (redir->type == IN && in_redir(redir))
			return (KO);
		redir = redir->next;
	}
	return (OK);
}

bool	append(t_redirects *redir)
{
	if (!redir->filename)
		return (KO);
	if (redir->type == APPEND)
		redir->fd[1] = open(redir->filename, O_WRONLY | O_CREAT
				| O_APPEND, 0644);
	else
		redir->fd[1] = open(redir->filename, O_WRONLY | O_CREAT
				| O_TRUNC, 0644);
	if (redir->fd[1] == -1)
	{
		perror(redir->filename);
		return (KO);
	}
	if (dup2(redir->fd[1], STDOUT_FILENO) == -1)
		return (KO);
	close(redir->fd[1]);
	return (OK);
}

bool	in_redir(t_redirects *redir)
{
	redir->fd[0] = open(redir->filename, O_RDONLY);
	if (redir->fd[0] == -1)
	{
		perror(redir->filename);
		return (KO);
	}
	if (dup2(redir->fd[0], STDIN_FILENO) == -1)
		return (KO);
	close(redir->fd[0]);
	return (OK);
}
