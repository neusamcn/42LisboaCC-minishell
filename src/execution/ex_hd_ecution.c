/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_hd_ecution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 04:20:55 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 13:43:35 by megiazar         ###   ########.fr       */
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

void	child_hd(t_redirects *r, int pipefd[2], t_shelly *shelly)
{
	char	*m;

	close(pipefd[0]);
	set_heredoc_signals(shelly);
	rl_catch_signals = 0;
	while (1)
	{
		m = readline("> ");
		if (!m || g_signal_stat == 130 || ft_strcmp(m, r->delimiter) == 0)
		{
			if (!m && g_signal_stat != 130)
				mndp_log_err(HD, r->delimiter);
			free(m);
			break ;
		}
		write_hd_line(m, pipefd[1], shelly, r->heredoc_quoted);
	}
	close(pipefd[1]);
	free_cmd_line(shelly->cur_cmd);
	babies_cleanup(shelly, NULL);
	free(shelly);
	if (g_signal_stat == 130)
		exit(130);
	exit(0);
}

void	mnd_heredoc(t_redirects *redir, t_shelly *shelly)
{
	pid_t			pid;
	int				status;
	int				pipefd[2];
	struct termios 	s_tty;
	int    			tty_sv;

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
		child_hd(redir, pipefd, shelly);
	sig_mode(MNDWAIT, shelly);
	waitpid(pid, &status, 0);
	sig_mode(INTERACTIVE, shelly);
	if (tty_sv)
		tcsetattr(STDIN_FILENO, TCSADRAIN, &s_tty);
	close(pipefd[1]);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		close(pipefd[0]);
		redir->xd_fd = -1;
		set_signal_stat(130);
		return ;
	}
	redir->xd_fd = dup(pipefd[0]);
	close(pipefd[0]);
}
