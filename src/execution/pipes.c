/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/04/01 20:25:54 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "execution.h"

/* ls stdout ──pipe1──► grep stdin
              grep stdout ──pipe2──► wc stdin
                                      wc stdout ──► terminal */		
									  
//TODO split functions
void heredoc(t_redirects *redir)
{
    int pipefd[2];
    char *prompt;
    
    prompt = NULL;
    pipe(pipefd);
    signal(SIGINT, sigint_glob);
    signal(SIGQUIT, SIG_IGN);
	g_signal = 0;
    while (1)
    {
        prompt = readline("> ");
        if (!prompt && g_signal == 130)
            exit(130);
        if (!prompt)
        {
            p("minishell: warning: here-document delimited by "
              "end-of-file (wanted '%s')\n", redir->delimiter);
            break ;
        }
		if (ft_strcmp(prompt, redir->delimiter) == 0)
			break ;
		write(pipefd[1], prompt, ft_strlen(prompt));
		write(pipefd[1], "\n", 1);
		free(prompt);
    }
	signal_handler(1);
    close(pipefd[1]);
	dup2(pipefd[0], STDIN);
	close(pipefd[0]);
}

void sigint_glob(int sig)
{
    (void)sig;
    g_signal = 130;
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
			p("minishell: ");
			perror("");
			exit(1);
		}
		dup2(fd[1], STDOUT);
        close(fd[1]);
    }
}

void pipe_handler(t_redirects *redir)
{
	int fd[2];
	
    if (redir->type == APPEND || redir->type == OUT)
		append(redir);
    if (redir->type == HEREDOC)
		heredoc(redir);
	else if (redir->type == IN)
    {
        if (access(redir->filename, F_OK) != 0)
        {
            p("minishell: %s: No such file or directory\n", redir->filename);
            return ;
        }
        fd[0] = open(redir->filename, O_RDONLY);
        if (fd[0] != -1)
            dup2(fd[0], STDIN);
        close(fd[0]);
    }
}

/* void pipe_handler(t_redirects *redir)
{
    int fd;

    if (redir->type == HEREDOC)
        heredoc(redir);
    else if (redir->type == IN)  // < infile
    {
        if (access(redir->filename, F_OK) != 0)
        {
            p("minishell: %s: No such file or directory\n", redir->filename);
            return ;
        }
        fd = open(redir->filename, O_RDONLY);
        if (fd != -1)
            dup2(fd, STDIN);
        close(fd);
    }
    else if (redir->type == OUT || redir->type == APPEND)
    {
        fd = open(redir->filename, O_WRONLY | O_CREAT |
            (redir->type == APPEND ? O_APPEND : O_TRUNC), 0644);
        if (fd == -1)
        {
            p("minishell: %s: No such file or directory\n", redir->filename);
            return ;
        }
        dup2(fd, STDOUT);
        close(fd);
    }
} */
