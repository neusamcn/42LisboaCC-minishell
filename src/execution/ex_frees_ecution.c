/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_frees_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:09:51 by megi              #+#    #+#             */
/*   Updated: 2026/05/19 20:12:32 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"


void	ft_free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	free_path(char **paths)
{
	int	j;

	j = 0;
	if (!paths)
		return (0);
	while (paths[j])
		free(paths[j++]);
	free(paths);
	return (1);
}

void	pipe_cl(t_cmd_line *pipeline)
{
	if (pipeline->prevfd != -1)
		close(pipeline->prevfd);
	if (pipeline->next)
	{
		close(pipeline->pipefd[1]);
		pipeline->next->prevfd = pipeline->pipefd[0];
		//ft_printf("pipe_cl: next->prevfd = %d\n", pipeline->next->prevfd);
	}
}
/* 
void	pipe_cl(t_cmd_line *pipeline)
{
	if (pipeline->prevfd != -1)
		close(pipeline->prevfd);
	if (pipeline->next)
	{
		close(pipeline->pipefd[1]);
		pipeline->next->prevfd = pipeline->pipefd[0];
	}
} */

void	free_cmd_line(t_cmd_line *cmd)
{
	t_cmd_line	*next;

	if (!cmd)
		return ;
	while (cmd)
	{
		next = cmd->next;
		ft_free_split(cmd->cmds);
		free_redirs(cmd->redir);
		free(cmd);
		cmd = next;
	}
}

