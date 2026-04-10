/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/04/08 19:48:28 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "execution.h"

//PUSH IN MAIN BRUNCH
//TODO: use minienvp instead of envp
int exec(t_pipe *cmd_line, char **envp, int status)
{
    char    *cmd;
    pid_t   pid;

	//if (are_you_builtin(cmd_line) == BUILTINS)
		//break ; //delete later
		//return run_builtins(cmd_line);
    cmd = path(cmd_line, envp);
    if (!cmd)
    {
        if (!ft_strchr(cmd_line->cmds[0], '/'))
            p_log_err(cmd_line->cmds[0], "command not found");
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
		parent_exec(status, 1);
    free(cmd);
    status = status_check(status);
    return status;
}

void child_exec(char *cmd, t_pipe *cmd_line, char **envp)
{
	set_signals_noninteractive();
    pipe_handler(&cmd_line->redir);
    if (execve(cmd, cmd_line->cmds, envp) == -1)
    {
		print_err_msg(cmd_line->cmds[0]);
        free(cmd);
        exit(127);
    }
}

void	parent_exec(int status, pid_t pid)
{	
	signal(SIGINT, sigint_glob);
	signal(SIGQUIT, SIG_IGN); 
	set_signal_stat(0);
	waitpid(pid, &status, 0);
	set_signals_interactive_parent();
}

int status_check(int status)
{
    if (WIFEXITED(status))
        status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        status = 128 + WTERMSIG(status);
    return status;
}

int are_you_builtin(t_pipe *cmd_line)
{
	if ((!ft_strcmp(cmd_line->cmds[0], CD)) || (!ft_strcmp(cmd_line->cmds[0], ECHO)) || (!ft_strcmp(cmd_line->cmds[0], EXIT)) ||
		(!ft_strcmp(cmd_line->cmds[0], PWD)) || (!ft_strcmp(cmd_line->cmds[0], ENV)) || (!ft_strcmp(cmd_line->cmds[0], EXPORT)) ||
		(!ft_strcmp(cmd_line->cmds[0], UNSET)))
        return (BUILTINS);
    return (NON_BUILTINS);
}
