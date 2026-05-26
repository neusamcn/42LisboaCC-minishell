/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_hd_ecution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 04:20:55 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 00:58:39 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

void	run_xds(t_cmd_line *cmds, t_shelly *shelly)
{
	t_cmd_line	*tmp;
	t_redirects	*r;

	tmp = cmds;
	while (tmp)
	{
		r = tmp->redir;
		while (r)
		{
			if (r->type == HEREDOC)
				mnd_heredoc(r, shelly);
			r = r->next;
		}
		tmp = tmp->next;
	}
}

void	write_hd_line(char *msg, int fd, t_shelly *shelly, bool quoted)
{
	char	*line;

	line = msg;
	if (!quoted)
		line = word_param_expansion(msg, shelly);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	if (!quoted)
		free(line);
	free(msg);
}

void	child_hd(t_redirects *redir, int pipefd[2], t_shelly *shelly)
{
	char	*msg;

	close(pipefd[0]);
	// sig_mode(CHILD, shelly);
	set_heredoc_signals(shelly);
	while (1)
	{
		msg = readline("> ");
		if (!msg || g_signal_stat == 130
			|| ft_strcmp(msg, redir->delimiter) == 0)
		{
			if (!msg)
				mndp_log_err(HD, redir->delimiter);
			free(msg);
			break ;
		}
		write_hd_line(msg, pipefd[1], shelly, redir->heredoc_quoted);
	}
	close(pipefd[1]);
	if (g_signal_stat == 130)
		exit(130);
	free_cmd_line(shelly->cur_cmd);
	babies_cleanup(shelly, NULL);
	free(shelly);
	exit(0);
}

void	mnd_heredoc(t_redirects *redir, t_shelly *shelly)
{
	pid_t	pid;
	int		status;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return ;
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return ;
	}
	if (pid == 0)
		child_hd(redir, pipefd, shelly);
	sig_mode(MNDWAIT, shelly);
	waitpid(pid, &status, 0);
	sig_mode(INTERACTIVE, shelly);
	close(pipefd[1]);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		redir->xd_fd = -1;
		set_signal_stat(130);
		redraw_prompt();
		return ;
	}
	redir->xd_fd = dup(pipefd[0]);
	close(pipefd[0]);
}
