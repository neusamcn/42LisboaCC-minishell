/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 02:47:24 by megi             ###   ########.fr       */
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
    if (if_redir(cmds))
        do_redri(cmds);
	else if (cmds->next == NULL && are_you_builtin(cmds) == BUILTINS)
		lonely_blt(cmds, envp);              // in a parent process, no fork
	else if (cmds->next == NULL)
		mommy_n_father(cmds, envp); // fork + execve
	else 
		ex_pipeline_ec(cmds, envp); // fork everything
}

int lonely_blt(t_cmd_line *s, char **envp)
{
	int	fd[2];

	(void)envp;
	if (if_redir(s))
	{
		fd[0] = dup(STDIN_FILENO);
		fd[1] = dup(STDOUT_FILENO);
		if (fd[0] == -1 || fd[1] == -1)
			return(perror("dup"), 1);
		do_redri(s);
	}
	sig_mode(BLT_EXECUTING);
//	r_bltn(s); // TODO: func for run a bltn
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

int mommy_n_father(t_cmd_line *s_cmd, char **envp)
{
	int status;
	pid_t only_child;
	char *path;

	status = 0;
	path = abs_or_rel_p(s_cmd, envp);
	//p("hb");
	if (!path)
		return (127);
	only_child = fork();
	//p("after");
	if (only_child == -1)
		return (perror("fork"), free(path), 1);
	if (only_child == 0)
		single_child_ex(path, s_cmd, envp);
	sig_mode(MNDWAIT);
	waitpid(only_child, &status, 0); // TODO: parent_wait function
	set_signals_interactive_parent();
	free(path);
	return (status_check(status));
}

void single_child_ex(char *cmd, t_cmd_line *kid, char **envp)
{
    sig_mode(CHILD);
    if (if_redir(kid))
		do_redri(kid);
    execve(cmd, kid->cmds, envp);
    print_err_msg(kid->cmds[0]);
    exit(127);
}
