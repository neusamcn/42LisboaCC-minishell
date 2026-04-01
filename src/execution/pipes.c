/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:33:48 by megi              #+#    #+#             */
/*   Updated: 2026/04/01 17:02:48 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "execution.h"

/* ls stdout ──pipe1──► grep stdin
              grep stdout ──pipe2──► wc stdin
                                      wc stdout ──► terminal */						  
void heredoc(t_pipe *cmd_line)
{
    int pipefd[2];
    char *prompt;
    
    prompt = NULL;
    pipe(pipefd);
    signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
    while (1)
    {
        prompt = readline("> ");
        if (!prompt || ft_strcmp(prompt, cmd_line->delimeter) == 0)
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

void append(t_pipe *cmd_line)
{
	int fd[2];

	if (cmd_line->outfile)
	{
		if ((access(cmd_line->infile, F_OK)) != 0)
		{
			p("minishell: %s: No such file or directory\n", cmd_line->outfile);
			exit (1);
		}
		if (cmd_line->append)
			fd[1] = open(cmd_line->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (cmd_line->outfile)
			fd[1] = open(cmd_line->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd[1] != -1)
			dup2(fd[1], STDOUT);
		close(fd[1]);
	}
}

void pipe_handler(t_pipe *cmd_line)
{
    int fd[2];

	if (cmd_line->outfile)
		append(cmd_line);
	if (cmd_line->heredoc)
		heredoc(cmd_line);
	else if (cmd_line->infile)
	{
		if ((access(cmd_line->infile, F_OK)) != 0)
			p("minishell: %s: No such file or directory\n", cmd_line->infile);
		fd[0] = open(cmd_line->infile, O_RDONLY);
    	if (fd[0] != -1)
			dup2(fd[0], STDIN);
    	close(fd[0]);			
	}
}

