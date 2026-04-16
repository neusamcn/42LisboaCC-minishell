/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/04/16 20:31:57 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

//PUSH IN MAIN BRUNCH
//TODO: use minienvp instead of envp

// builtin or external cmd?
// if external - check if the path is absolute -> find a path or relative->use it
// run builtins+single cmd->no fork 
// run external-> fork + execve

void exec_loop(t_cmd_line *cmds, char **envp)
{
	// check if its no cmd, only redirection
	if (cmds->next == NULL && are_you_builtin(cmds) == BUILTINS)
		lonely_blt(cmds, envp);              // in a parent process, no fork
	else if (cmds->next == NULL)
		mommy_n_father(cmds, envp); // fork + execve
	else
		ex_pipeline_ec(cmds, envp); // fork everything
}

int lonely_blt(t_cmd_line *s, char **envp)
{
	int	fd[2];

	if (if_redir(s))
	{
		fd[0] = dup(STDIN_FILENO);
		fd[1] = dup(STDOUT_FILENO);
		if (fd[0] == -1 || fd[1] == -1)
			return(perror("dup"), 1);
		do_redri(s);
	}
	sig_mode(BLT_EXECUTING);
	r_bltn(s); // TODO: func for run a bltn
	if (if_redir(s))
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	set_signals_interactive_parent();
	return (get_signal_stat());
}

//TODO: check statement at the parent mnd function
/* if (!cmd_line->cmds || !cmd_line->cmds[0])
{
	if (cmd_line->redir.type == HEREDOC)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			heredoc(&cmd_line->redir);
			exit(0);
		}
		waitpid(pid, NULL, 0);
	}
	return 0;
} */

int mommy_n_father(t_cmd_line *s_cmd, char **envp)
{
	int status;
	pid_t pid;
	char *path;

	status = 0;
	path = abs_or_rel_p(s_cmd, envp);
	if (!path)
		return (127);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), free(path), 1);
	if (pid == 0)
		child_ex(path, s_cmd, envp);
	sig_mode(MNDWAIT);
	waitpid(pid, &status, 0); // TODO: parent_wait function
	set_signals_interactive_parent();
	free(path);
	return (status_check(status));
}

char    *abs_or_rel_p(t_cmd_line *c, char **envp)
{
	char *p;
	
	p = c->cmds[0];
	if (!p)
	{
		p_log_err("Command not found\n");
		return (NULL);
	}
	if (ft_strchr(p, '/'))
		return (absolute_path(p));
	return(relative_path(c, envp));
}

bool if_redir(t_cmd_line *s)
{
	return (s->redir.type != NONE);
}

void do_redri(t_cmd_line *s)
{
	pipe_handler(&s->redir);
}
