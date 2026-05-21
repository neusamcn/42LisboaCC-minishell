/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 22:41:15 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/21 22:52:24 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static void	get_redir(t_token *tkns, t_cmd_line *cmd_line)
{
	if (!tkns || !cmd_line || tkns->type == CTRL_OP || tkns->ctrlop == PIPE)
		return ;
	cmd_line->redir = ft_calloc_protec(1, sizeof(t_redirects));
	// TODO: Create next and link nodes?
	if (tkns->type == REDIR)
	{
		cmd_line->redir->type = tkns->redir;
		cmd_line->redir->fd[0] = -1;
		cmd_line->redir->fd[1] = -1;
		cmd_line->redir->xd_fd = -1;
		if (tkns->redir == HEREDOC && tkns->next->type == WORD && tkns->next)
			cmd_line->redir->delimiter = ft_strdup(tkns->next->value);
		else if (tkns->redir == IN && tkns->previous
			&& tkns->previous->type == WORD)
			cmd_line->redir->filename = ft_strdup(tkns->previous->value);
		else if ((tkns->redir == OUT || tkns->redir == APPEND) && tkns->next)
			cmd_line->redir->filename = ft_strdup(tkns->next->value);
	}
	else
		cmd_line->redir->type = NONE;
}

static char	*cpy_cmd(t_token *tkns, t_cmd_line *cmd_line)
{
	char	*cmds;

	if (!tkns || !cmd_line || tkns->type == CTRL_OP)
		return (NULL);
	cmds = NULL;
	if (tkns->space_b4_word == true)
	{
		if (cmd_line->cmds == NULL)
			cmds = ft_strdup(" ");
		else
			cmds = ft_strjoin_free(cmd_line->cmds, " ");
		cmds = ft_strjoin_free(cmds, tkns->value);
	}
	else
	{
		if (cmd_line->cmds == NULL)
			cmds = ft_strdup(tkns->value);
		else
			cmds = ft_strjoin_free(cmd_line->cmds, tkns->value);
	}
	return (cmds);
}

t_cmd_line	*parser(t_token *tokens)
{
	t_cmd_line	*cmd_line_head;
	t_cmd_line	*curr_cmd;
	t_token		*curr_tkn;

	if (!tokens)
		return (NULL);
	curr_tkn = tokens;
	cmd_line_head = ft_calloc_protec(1, sizeof(t_cmd_line));
	curr_cmd = cmd_line_head;
	while (curr_tkn)
	{
		if (curr_tkn->type != CTRL_OP)
		{
			curr_cmd->cmds = cpy_cmd(curr_tkn, curr_cmd);
			get_redir(curr_tkn, curr_cmd);
		}
		else if (curr_tkn->type == CTRL_OP && curr_tkn->ctrlop == PIPE)
		{
			curr_cmd->next = ft_calloc_protec(1, sizeof(t_cmd_line));
			curr_cmd = curr_cmd->next;
		}
		curr_tkn = curr_tkn->next;
	}
	return (cmd_line_head);
}
