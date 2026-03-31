/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:26:32 by megi              #+#    #+#             */
/*   Updated: 2026/03/31 19:42:55 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "execution.h"

/* в child перед execve() обычно возвращают SIGINT и SIGQUIT к поведению 
по умолчанию (SIG_DFL). 
Тогда команда ведёт себя как в bash: Ctrl-C убивает текущую команду, 
а Ctrl-\ действует на неё, а не на сам shell. */

int exec(t_pipe *cmd_line, char **envp, int status)
{
    char    *cmd;
    pid_t   pid;
/*     if (are_you_builtin(cmd_line) == BUILTINS)
        return run_builtins(cmd_line); */
    cmd = path(cmd_line, envp);
	if (!cmd)
	{
		if (!ft_strchr(cmd_line->cmds[0], '/'))
			p("minishell: %s: command not found\n", cmd_line->cmds[0]);
        return 127;
    }
    signal_handler(1);
    pid = fork();
	if (pid == 0)
	child_exec(cmd, cmd_line, envp);
    else if (pid != 0)
	waitpid(pid, &status, 0);
	free(cmd);
    status = status_check(status);
    return status;
}

void	child_exec(char *cmd, t_pipe *cmd_line, char **envp)
{
	signal_handler(0);
	pipe_handler(cmd_line);
	if (execve(cmd, cmd_line->cmds, envp) == -1)
	{
		p("minishell: %s", cmd_line->cmds[0]);
		perror("");
		free(cmd);
		exit(127);
	}
}
/* 
Ign: Ignore the signal, do nothing just return.
Term: Terminate the process. ctrl+z
Cont: Unblock a stopped process.
Stop: Block the process.
 */

void signal_handler(pid_t pid)
{
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
    }
    else
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGSTOP, SIG_IGN); 
    }
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
