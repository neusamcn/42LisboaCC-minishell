/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/04/15 22:01:49 by megi             ###   ########.fr       */
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
		ex_lonely(cmds, envp); // fork + execve
	else
		exec_pipeline(cmds, envp);       // fork everything
}

int lonely_blt(t_cmd_line *s, char **envp)
{
	int	io[2];

	if (if_redir(s, envp))
	{
		io[0] = dup(STDIN_FILENO);
		io[1] = dup(STDOUT_FILENO);
	}
	sig_mode(BLT_EXECUTING);
	r_bltn(s); // TODO: func for run a bltn
	if (if_redir(s, envp))
	{
		dup2(io[0], STDIN_FILENO);
		dup2(io[1], STDOUT_FILENO);
		close(io[0]);
		close(io[1]);
	}
	set_signals_interactive_parent();
	return (get_signal_stat());
}

//todo a parent exec
int ex_lonely(t_cmd_line *s_cmd, char **envp)
{
	int status;
	pid_t pid;
	char *cmd;

	status = 0;
	cmd = abs_or_rel_p(s_cmd, envp);
	if (!cmd)
		return (127);
	pid = fork();
	if (pid == -1)
	{
		perror("fork: \n");
		exit (1);
	}
	if (pid == 0)
		child_ex(cmd, s_cmd, envp);
	sig_mode(MNDWAIT);
	waitpid(pid, &status, 0);
	set_signals_interactive_parent();
	free(cmd);
	return (status_check(status));
}

void child_ex(char *cmd, t_cmd_line *kid, char **envp)
{
	sig_mode(CHILD);
	f_redir(kid, envp);
	execve(cmd, kid->cmds, envp);
	exit(127);
}

int mommy_n_father(t_cmd_line *s, char **envp)
{
	int status;
	pid_t pid;
	char    *cmd;

	cmd = abs_or_rel_p(s, envp);
	pid = fork();
	if (pid == -1)
	{
		perror("fork: \n");
		exit (1);
	}
	if (pid == 0)
		child_ex(cmd, s, envp);
	sig_mode(MNDWAIT);
	waitpid(pid, &status, 0);
	sig_mode(INTERACTIVE);
	free(cmd);
	return (status_check(status));
}

bool if_redir(t_cmd_line *check, char **envp)
{
	if (check->redir.type == NONE)
		return (false);
	pipe_handler(&check->redir);
	return (true);
}

char    *abs_or_rel_p(t_cmd_line *c, char **envp)
{
	char *cm;

	cm = c->cmds[0];
	if (!cm)
	{
		p_log_err("Command not found\n");
		return (NULL);
	}
	if (ft_strchr(cm, '/'))
		return (absolute_path(cm));
	return(relative_path(c, envp));
}

/* 
int exec(t_cmd_line *cmd_line, char **envp)
{
	char    *cmd;
	pid_t   pid;
	int status;

	status = 0;
	if (!cmd_line->cmds || !cmd_line->cmds[0])
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
	}
	// check if the path is absolute or relative
	if (ft_strchr(cmd_line, '/'))
		cmd = path(cmd_line, envp);
	if (!cmd)
	{
		if (!ft_strchr(cmd_line->cmds[0], '/'))
			p_log_err(cmd_line->cmds[0]); // , "command not found"
		return 127;
	}
	pid = fork();
	if (pid == -1)
	{
		free(cmd);
		return (1);
	}
	if (pid == 0)
		child_exec(cmd, cmd_line, envp);
	else
		parent_exec(&status, pid);
	free(cmd);
	status = status_check(status);
	return status;
}

void child_exec(char *cmd, t_cmd_line *cmd_line, char **envp)
{
	//set_signals_noninteractive();
	pipe_handler(&cmd_line->redir);
	if (execve(cmd, cmd_line->cmds, envp) == -1)
	{
		print_err_msg(cmd_line->cmds[0]);
		free(cmd);
		exit(127);
	}
}

void	parent_exec(int *status, pid_t pid)
{	
	signal(SIGINT, sigint_glob);
	signal(SIGQUIT, SIG_IGN); 
	set_signal_stat(0);
	waitpid(pid, &status, 0);
	set_signals_interactive_parent();
} */