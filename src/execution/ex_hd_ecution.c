/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_hd_ecution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 04:20:55 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/24 12:17:26 by megiazar         ###   ########.fr       */
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
	sig_mode(CHILD);
	while (1)
	{
		msg = readline("> ");
		if (!msg || ft_strcmp(msg, redir->delimiter) == 0)
		{
			if (!msg)
				mndp_log_err(HD, redir->delimiter);
			free(msg);
			break ;
		}
		write_hd_line(msg, pipefd[1], shelly, redir->heredoc_quoted);
	}
	close(pipefd[1]);
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
	}
	if (pid == 0)
		child_hd(redir, pipefd, shelly);
	sig_mode(MNDWAIT);
	waitpid(pid, &status, 0);
	sig_mode(INTERACTIVE);
	close(pipefd[1]);
	redir->xd_fd = dup(pipefd[0]);
	close(pipefd[0]);
}
