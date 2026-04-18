/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_pipes_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 02:28:59 by megi             ###   ########.fr       */
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

static void pipe_cl(t_cmd_line *pipeline)
{
	if (pipeline->prevfd != -1)
		close(pipeline->prevfd);
	if (pipeline->next)
	{
		close(pipeline->pipefd[1]);
		pipeline->next->prevfd = pipeline->pipefd[0];
	}
}

int	ex_pipeline_ec(t_cmd_line *pipeline, char **envp)
{
	pid_t pid;
	int status;
	pid_t last_st;
	int cmd_num;
	char *path;

	pipeline->prevfd = -1;
	cmd_num = 0;
	while (pipeline)
	{
		path = relative_path(pipeline, envp);
		if (pipeline->next)
			pipe(pipeline->pipefd);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
    		child_ex(path, pipeline, envp);
		pipe_cl(pipeline);
		last_st = pid;
		cmd_num++;
		sig_mode(MNDWAIT);
		pipeline = pipeline->next;
	}
	status = mndwait(last_st, cmd_num);
	return (set_signal_stat(status), 1);
}

int mndwait(pid_t status_of_last_child, int cmd_nmb)
{
	pid_t pid;
	int status;
	int last_stat;

	status = 0;
	while (cmd_nmb > 0)
	{
		pid = wait(&status_of_last_child);
		if (pid == last_stat)
			last_stat = status;
		cmd_nmb--;
	}
	return (status);
}

void child_ex(char *cmd, t_cmd_line *kid, char **envp)
{
    sig_mode(CHILD);
	//p("inside the child\n");
    if (kid->prevfd != -1)
		dup2(kid->prevfd, STDIN_FILENO);
    if (kid->next != NULL)
		dup2(kid->pipefd[1], STDOUT_FILENO);
    if (kid->next != NULL)
    {
		close(kid->pipefd[0]);
        close(kid->pipefd[1]);
    }
	if (kid->prevfd != -1)
		close(kid->prevfd);
    if (if_redir(kid))
		do_redri(kid);
/* 	if (are_you_builtin(kid) == BUILTINS)
		exit(r_bltn(kid)); //todo wya */
    execve(cmd, kid->cmds, envp);
    print_err_msg(kid->cmds[0]);
    exit(127);
}

