/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_redir_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 23:41:36 by megi              #+#    #+#             */
/*   Updated: 2026/04/17 23:44:07 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

void pipe_handler(t_redirects *redir)
{
	int fd[2];
	
	if (redir->type == APPEND || redir->type == OUT)
		append(redir);
	else if (redir->type == HEREDOC)
		heredoc(redir);
	else if (redir->type == IN)
	{
		if (access(redir->filename, F_OK) != 0)
			p_log_err("No such file or directory\n"); // redir->filename ;
		fd[0] = open(redir->filename, O_RDONLY);
		if (fd[0] != -1)
		{
			dup2(fd[0], READ);
			close(fd[0]);	
		}
	}
}

void heredoc(t_redirects *redir)
{
	int pipefd[2];
	char *msg;
	
	pipe(pipefd);
	while (1)
	{
		msg = readline("> ");
		if (!msg)
		{
			p_log_err(HD);
			break ;
		}
		if (strcmp(msg, redir->delimiter) == TRUE)
		{
			free(msg);
			break ;
		}
		write(pipefd[1], msg, ft_strlen(msg));
		write(pipefd[1], "\n", 1);
		free(msg);
	}
	close(pipefd[1]);
	dup2(pipefd[0], READ);
	close(pipefd[0]);
}

void append(t_redirects *redir)
{
	int fd[2];
	
    if (redir->filename)
    {
		if (redir->type == APPEND)
			fd[1] = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
			fd[1] = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd[1] == -1)
		{
			print_err_msg(redir->filename);
			exit(1);
		}
		dup2(fd[1], WRITE);
        close(fd[1]);
    }
}