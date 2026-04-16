/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_pipes_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/04/16 20:32:12 by megi             ###   ########.fr       */
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

int	ex_pipeline_ec(t_cmd_line *pipeline, char *envp)
{
	pid_t pid;
	int status;

	pipeline->prevfd = -1;
	while (pipeline)
	{
		if (pipeline->next)
			pipe(pipeline->pipefd);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
				child_ex(0, pipeline, envp);
		if (pipeline->prevfd != -1)
			close(pipeline->prevfd);
		if (pipeline->next)
		{
			close(pipeline->pipefd[1]);
			pipeline->pipefd[0] = pipeline->prevfd;
		}
		sig_mode(MNDWAIT);
		pipeline = pipeline->next;
	}
	waitpid(pid, &status, 0); //TODO parent_wait func
	return (set_signal_stat(status), 1);
}


void child_ex(char *cmd, t_cmd_line *kid, char **envp)
{
    sig_mode(CHILD);
    if (kid->prevfd != -1)
	dup2(kid->prevfd, STDIN_FILENO);
    if (kid->is_there_more_pipes == true)
	dup2(kid->pipefd[1], STDOUT_FILENO);
    if (kid->is_there_more_pipes == true)
    {
		close(kid->pipefd[0]);
        close(kid->pipefd[1]);
    }
	if (kid->prevfd != -1)
	close(kid->prevfd);
    if (if_redir(kid))
		do_redri(kid);
	if (are_you_builtin(kid) == BUILTINS)
		exit(r_bltn(kid));
    execve(cmd, kid->cmds, envp);
    print_err_msg(kid->cmds[0]);
    exit(127);
}

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


//TODO: in a exec success
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

