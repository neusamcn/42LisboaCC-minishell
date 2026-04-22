/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_redir_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 23:41:36 by megi              #+#    #+#             */
/*   Updated: 2026/04/22 15:36:46 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

int which_redir_type(t_redirects *redir)
{
	while (redir && redir->type != NONE)
	{
        if (redir->type == HEREDOC)
            dup2(redir->xd_fd, READ);
		else if (redir->type == APPEND || redir->type == OUT)
			append(redir);
		else if (redir->type == IN)
		{
			if (in_redir(redir) != 0)
        		return (1);
		}
		redir = redir->next;
	}
	return (0);
}

void append(t_redirects *redir)
{
    if (redir->filename)
    {
		if (redir->type == APPEND)
			redir->fd[1] = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
			redir->fd[1] = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (redir->fd[1] == -1)
			print_err_msg(redir->filename);
		dup2(redir->fd[1], WRITE);
        close(redir->fd[1]);
    }
}

int in_redir(t_redirects *redir)
{
	if (redir->type == IN)
	{
		if (access(redir->filename, F_OK) != 0)
		{
			mndp_log_err("No such file or directory\n", redir->filename);	
			return (1);
		}
		redir->fd[0] = open(redir->filename, O_RDONLY);
		if (redir->fd[0] != -1)
		{
			dup2(redir->fd[0], READ);
			close(redir->fd[0]);	
		}
	}
	return (0);
}

static void child_hd(t_redirects *redir, int pipefd[2])
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
		if (ft_strcmp(msg, redir->delimiter) == TRUE)
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

void heredoc(t_redirects *redir)
{
    int     pipefd[2];
    pid_t   pid;
    int     status;

    pipe(pipefd);
    pid = fork();
    if (pid == 0)
        child_hd(redir, pipefd);
    sig_mode(MNDWAIT);
    waitpid(pid, &status, 0);
    sig_mode(INTERACTIVE);
    close(pipefd[1]);
    redir->xd_fd = dup(pipefd[0]);
    close(pipefd[0]);
}
