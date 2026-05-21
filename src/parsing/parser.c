/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 22:41:15 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/21 01:11:52 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static char	*get_delimiter(t_token *tkns)
{
	t_token	*curr_tkn;
	char	*delimiter;

	curr_tkn = tkns;
	while (tkns->index != 0)
		curr_tkn = curr_tkn->next;
	if (curr_tkn->index == 0)
		curr_tkn = curr_tkn ->previous;
	if (curr_tkn->index > 0)
		delimiter = curr_tkn->value;
	return (delimiter);
}

static char	*cpy_cmds(t_token *tkns, t_cmd_line **cmd_line)
{
	char	*cmds;

	cmds = ft_strdup("");
	while (tkns && tkns->type != CTRL_OP)
	{
		cmds = ft_strjoin_free(cmds, tkns->value);
		if (tkns->type == REDIR)
		{
			(*cmd_line)->redir->type = tkns->redir;
			(*cmd_line)->redir->fd[2] = -1;
			(*cmd_line)->redir->xd_fd = -1;
			if (tkns->redir == HEREDOC)
			{
				(*cmd_line)->redir->filename = NULL;
				(*cmd_line)->redir->delimiter = get_delimiter(tkns);
			}
			else if (tkns->redir == IN)
				(*cmd_line)->redir->filename = tkns->previous->value;
			else if (tkns->redir == OUT || tkns->redir == APPEND)
				(*cmd_line)->redir->filename = tkns->next->value;
		}
		else
			(*cmd_line)->redir->type = NONE;
		// TODO: connect the nodes if pipe comes after
		tkns = tkns->next;
	}
	return (cmds);
}


t_cmd_line	*parser(t_token *tokens)
{
	t_cmd_line	*cmd_line;
	t_token		*curr_tkn;
	char		*tmp;

	if (!tokens)
		return (NULL);
	curr_tkn = tokens;
	while (curr_tkn)
	{
		if (curr_tkn->type != CTRL_OP)
		{
			cmd_line = ft_calloc_protec(1, sizeof(t_cmd_line));
			cmd_line->cmds = cpy_cmds(curr_tkn, &cmd_line);
		}
	}
	return (cmd_line);
}
