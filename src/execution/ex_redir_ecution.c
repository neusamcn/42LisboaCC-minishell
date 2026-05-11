/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_redir_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 23:41:36 by megi              #+#    #+#             */
/*   Updated: 2026/05/11 13:21:47 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/*
Input/output redirections && HD for a single cmd in a pipeline
Redirections modify how a cmd reads input and writes output by changing
its file descriptors before execution.
A cmd may have:
		NONE, // 0
		IN,  // < redir input to a cmd, taking input from a file
		OUT, // > redir output to a file, and overwrites the file if it already exists
		APPEND, // >> redir output top a file, append the output to the end of the file
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

// true means error and false means success!
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
				return (true);
			dup2(redir->xd_fd, STDIN_FILENO);
			close(redir->xd_fd);
		}
		else if (redir->type == APPEND || redir->type == OUT)
		{
			if (append(redir))
				return (true);
		}
		else if (redir->type == IN && in_redir(redir))
			return (true);
		redir = redir->next;
	}
	return (false);
}

bool	append(t_redirects *redir)
{
	if (!redir->filename)
		return (true);
	if (redir->type == APPEND)
		redir->fd[1] = open(redir->filename, O_WRONLY | O_CREAT
				| O_APPEND, 0644);
	else
		redir->fd[1] = open(redir->filename, O_WRONLY | O_CREAT
				| O_TRUNC, 0644);
	if (redir->fd[1] == -1)
	{
		perror(redir->filename);
		return (true);
	}
	if (dup2(redir->fd[1], STDOUT_FILENO) == -1)
		return (true);
	close(redir->fd[1]);
	return (false);
}

bool	in_redir(t_redirects *redir)
{
	redir->fd[0] = open(redir->filename, O_RDONLY);
	if (redir->fd[0] == -1)
	{
		perror(redir->filename);
		return (true);
	}
	if (dup2(redir->fd[0], STDIN_FILENO) == -1)
		return (true);
	close(redir->fd[0]);
	return (false);
}

static void	child_hd(t_redirects *redir, int pipefd[2])
{
	char	*msg;

	close(pipefd[0]);
	sig_mode(CHILD);
	while (1)
	{
		msg = readline("> ");
		if (!msg)
		{
			mndp_log_err(HD, redir->delimiter);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strcmp(msg, redir->delimiter) == 0)
		{
			free(msg);
			break ;
		}
		write(pipefd[1], msg, ft_strlen(msg));
		write(pipefd[1], "\n", 1);
		free(msg);
	}
	close(pipefd[1]);
	exit(0);
}

void	heredoc(t_redirects *redir)
{
	pid_t	pid;
	int		status;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		child_hd(redir, pipefd);
	sig_mode(MNDWAIT);
	waitpid(pid, &status, 0);
	sig_mode(INTERACTIVE);
	close(pipefd[1]);
	redir->xd_fd = dup(pipefd[0]);
	close(pipefd[0]);
}
