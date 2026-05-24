/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 22:41:15 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/24 18:09:20 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (ft_calloc_protec(1, new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = ft_calloc_protec(1, new_size);
	ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}

static void	append_redir(t_cmd_line *cmd_line, t_redirects *new_r)
{
	t_redirects	*last;

	if (!cmd_line->redir)
		cmd_line->redir = new_r;
	else
	{
		last = cmd_line->redir;
		while (last->next)
		last = last->next;
		last->next = new_r;
	}
}

static void	get_redir(t_token *tkns, t_cmd_line *cmd_line)
{
	t_redirects	*new_r;

	if (!tkns || !cmd_line || tkns->type != REDIR)
		return ;
	new_r = ft_calloc_protec(1, sizeof(t_redirects));
	new_r->type = tkns->redir;
	new_r->fd[0] = -1;
	new_r->fd[1] = -1;
	new_r->xd_fd = -1;
	if (tkns->redir == HEREDOC && tkns->next && tkns->next->type == WORD)
	{
		new_r->heredoc_quoted = (tkns->next->word != CMD);
		new_r->delimiter = word_param_expansion(tkns->next->value, NULL);
	}
	else if ((tkns->redir == IN || tkns->redir == OUT
		|| tkns->redir == APPEND) && tkns->next)
		new_r->filename = ft_strdup(tkns->next->value);
	append_redir(cmd_line, new_r);
}

static void	cpy_cmd(t_token *tkns, t_cmd_line *cmd_line)
{
	int		i;
	char	**new_cmds;

	if (!tkns || !cmd_line || tkns->type == CTRL_OP)
		return ;
	i = 0;
	while (cmd_line->cmds && cmd_line->cmds[i])
		i++;
	if (!cmd_line->cmds || tkns->space_b4_word == false)
	{
		new_cmds = ft_realloc(cmd_line->cmds, i
				* sizeof(char *), (i + 2) * sizeof(char *));
		cmd_line->cmds = new_cmds;
		cmd_line->cmds[i] = ft_strdup(tkns->value);
		cmd_line->cmds[i + 1] = NULL;
	}
	else
		cmd_line->cmds[i - 1] = ft_strjoin_free(cmd_line->cmds[i - 1],
				tkns->value);
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
		if (curr_tkn->type == WORD && !(curr_tkn->previous
				&& curr_tkn->previous->type == REDIR))
			cpy_cmd(curr_tkn, curr_cmd);
		else if (curr_tkn->type == REDIR)
			get_redir(curr_tkn, curr_cmd);
		else if (curr_tkn->type == CTRL_OP && curr_tkn->ctrlop == PIPE)
		{
			curr_cmd->next = ft_calloc_protec(1, sizeof(t_cmd_line));
			curr_cmd = curr_cmd->next;
		}
		curr_tkn = curr_tkn->next;
	}
	return (cmd_line_head);
}
