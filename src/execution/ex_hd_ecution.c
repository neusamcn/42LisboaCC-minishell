/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_hd_ecution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 04:20:55 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 17:09:42 by megiazar         ###   ########.fr       */
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

void	child_hd(t_redirects *r, int pipefd[2], t_shelly *shelly)
{
	char	*m;

	close(pipefd[0]);
	set_heredoc_signals(shelly);
	while (1)
	{
		m = readhd();
		if (!m || g_signal_stat == 130 || ft_strcmp(m, r->delimiter) == 0)
		{
			if (!m && g_signal_stat != 130)
				mndp_log_err(HD, r->delimiter);
			free(m);
			break ;
		}
		writehd(m, pipefd[1], shelly, r->heredoc_quoted);
	}
	close(pipefd[1]);
	free_cmd_line(shelly->cur_cmd);
	babies_cleanup(shelly, NULL);
	free(shelly);
	if (g_signal_stat == 130)
		exit(130);
	exit(0);
}

static void	mndhd_wait(t_redirects *r, t_shelly *shelly, struct termios *s_tty,
		int tty_sv)
{
	int		status;
	pid_t	pid;
	int		pipefd[2];

	sig_mode(MNDWAIT, shelly);
	waitpid(pid, &status, 0);
	sig_mode(INTERACTIVE, shelly);
	if (tty_sv)
		tcsetattr(STDIN_FILENO, TCSADRAIN, s_tty);
	close(pipefd[1]);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		close(pipefd[0]);
		r->xd_fd = -1;
		set_signal_stat(130);
		return ;
	}
	r->xd_fd = dup(pipefd[0]);
	close(pipefd[0]);
}

void	mnd_heredoc(t_redirects *redir, t_shelly *shelly)
{
	pid_t			pid;
	int				pipefd[2];
	struct termios	s_tty;
	int				tty_sv;

	tty_sv = (tcgetattr(STDIN_FILENO, &s_tty) == 0);
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
	{
		child_hd(redir, pipefd, shelly);
		exit(0);
	}
	mndhd_wait(redir, pipefd, shelly, &s_tty, tty_sv);
}
