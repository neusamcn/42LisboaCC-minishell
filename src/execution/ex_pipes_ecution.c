/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_pipes_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/04/15 21:06:53 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/* ls stdout ──pipe1──► grep stdin
              grep stdout ──pipe2──► wc stdin
                                      wc stdout ──► terminal */		
									  
//TODO split functions

//do redir tests
/* ls > files.txt
echo "some text" >> files.txt
sort < files.txt  */

//Input piping
//Output piping allows to pass the output of one cmds as input to anotehr cmd

// we can also pipe the output of a here doc into another cmd

/* cat << EOF | grep "grr"
cat << EOF > file.txt
<< EOF
echo "hi" << EOF
ftp -n << EOF FILE TRANSFER PROTOCOL can accept cmds via HD for automatic file
bc << EOF bc cmd is a calculator that can be used with here docs to perform calloc
sendmail user@mail.ru << end */

//create a child process
//why would we ignore expansions Literal Text
//cat << 'EOF'
//TODO : expands from pars

//TODO hd msg after output should be before)
//TODO when its no cmd before it should be msg

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
			p_log_err(redir->filename); // "No such file or directory";
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
			//write(pipefd[1], "\n", 1);
			break ;
		}
		if (strcmp(msg, redir->delimiter) == TRUE)
		{
			free (msg);
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

void	close_fds(void)
{
	int fd[2];

	if (fd[0] == FD_OPEN)
	{
		close(fd[0]);
		fd[0] = FD_CLOSED;
	}
	if (fd[1] == FD_OPEN)
	{
		close(fd[1]);
		fd[1] = FD_CLOSED;
	}
}

/* void	apply_fd(int fd[2])
{
	if (fd[0] != -1)
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			exit(1);
		}
	}
	if (fd[1] != -1)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			exit(1);
		}
	}
}

/* 
void	init_fds(t_redirects *check)
{
	check->io[STDIN] = -1;
	check->io[STDOUT] = -1;
	check->fd_state[STDIN] = FD_CLOSED;
	check->fd_state[STDOUT] = FD_CLOSED;
}

int	fd_set(t_redirects *c, int io_type, int fd)
{
	if (fd >= 0)
	{
		c->io[io_type] = fd;
		c->fd_state[io_type] = FD_OPEN;
	}
}

void fd_close_all(t_redirects *c, int fd)
{
	if (fd >= 0)
	{
		if (c->fd_state[STDIN] == FD_OPEN)
		{
			close(c->io[STDIN]);
			c->fd_state[STDIN] = FD_CLOSED;
		}
		if (c->fd_state[STDOUT] == FD_OPEN)
		{
			close(c->io[STDOUT]);
			c->fd_state[STDOUT] = FD_CLOSED;
		}
	}
} */
/* void	connect_fd(t_redirects *r, int io_type, int std)
{
	if (r->fd_state[io_type] == FD_OPEN)
	{
		if (dup2(r->io[io_type], std) == -1)
		{
			perror("dup2");
			exit(1);
		}
	}
} */
