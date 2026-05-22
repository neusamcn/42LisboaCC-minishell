/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 22:41:15 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/22 17:22:05 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static void    *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void    *new_ptr;

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

static void	get_redir(t_token *tkns, t_cmd_line *cmd_line)
{
	if (!tkns || !cmd_line || tkns->type != REDIR)
		return ;
	cmd_line->redir = ft_calloc_protec(1, sizeof(t_redirects));
	// TODO: Create next and link nodes?
	if (tkns->type == REDIR)
	{
		cmd_line->redir->type = tkns->redir;
		cmd_line->redir->fd[0] = -1;
		cmd_line->redir->fd[1] = -1;
		cmd_line->redir->xd_fd = -1;
		if (tkns->redir == HEREDOC && tkns->next && tkns->next->type == WORD)
			cmd_line->redir->delimiter = ft_strdup(tkns->next->value);
		else if (tkns->redir == IN && tkns->next)
			cmd_line->redir->filename = ft_strdup(tkns->next->value);
		else if ((tkns->redir == OUT || tkns->redir == APPEND) && tkns->next)
				cmd_line->redir->filename = ft_strdup(tkns->next->value);
	}
	else
		cmd_line->redir->type = NONE;
}

static void cpy_cmd(t_token *tkns, t_cmd_line *cmd_line)
{
	int     i;
	char    **new_cmds;
			
	if (!tkns || !cmd_line || tkns->type == CTRL_OP)
		return ;
	i = 0;
	while (cmd_line->cmds && cmd_line->cmds[i])
		i++;
	if (!cmd_line->cmds || tkns->space_b4_word == false)
	{
    	new_cmds = ft_realloc(cmd_line->cmds, i * sizeof(char *), (i + 2) * sizeof(char *));
        cmd_line->cmds = new_cmds;
        cmd_line->cmds[i] = ft_strdup(tkns->value);
        cmd_line->cmds[i + 1] = NULL;
    }
    else
	cmd_line->cmds[i - 1] = ft_strjoin_free(cmd_line->cmds[i - 1], tkns->value);
}

t_cmd_line	*parser(t_token *tokens)
{
	t_cmd_line	*cmd_line_head;
	t_cmd_line	*curr_cmd;
	t_token		*curr_tkn;
	//int i;

	if (!tokens)
		return (NULL);
	curr_tkn = tokens;
	cmd_line_head = ft_calloc_protec(1, sizeof(t_cmd_line));
	curr_cmd = cmd_line_head;
	while (curr_tkn)
	{
		if (curr_tkn->type == WORD)
		{
			if (!(curr_tkn->previous && curr_tkn->previous->type == REDIR))
				cpy_cmd(curr_tkn, curr_cmd);
		}
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


/* static char	*cpy_cmd(t_token *tkns, t_cmd_line *cmd_line)
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
			cmds = ft_strjoin_free(*cmd_line->cmds, " ");
		cmds = ft_strjoin_free(cmds, tkns->value);
	}
	else
	{
		if (cmd_line->cmds == NULL)
			cmds = ft_strdup(tkns->value);
		else
			cmds = ft_strjoin_free(*cmd_line->cmds, tkns->value);
	}
	return (cmds);
} */

/* static int count_cmd_tkns(t_token *tkns)
{
	int i;

	i = 0;
	while (tkns && tkns->type != CTRL_OP)
	{
		if (tkns->type == WORD)
		i++;
		tkns = tkns->next;
	}
	return (i);
} */

/* t_cmd_line *parser(t_token *tokens)
{
	t_cmd_line *cmd_line_head;
	t_cmd_line *curr_cmd;
	t_token  *curr_tkn;
	int   i;

	if (!tokens)
		return (NULL);
	curr_tkn = tokens;
	cmd_line_head = ft_calloc_protec(1, sizeof(t_cmd_line));
	curr_cmd = cmd_line_head;
	curr_cmd->cmds = ft_calloc_protec(count_cmd_tkns(curr_tkn) + 1, sizeof(char *));
	i = 0;
	while (curr_tkn)
	{
		if (curr_tkn->type != CTRL_OP)
		{
			if (curr_tkn->type == WORD)
				curr_cmd->cmds[i++] = cpy_cmd(curr_tkn, curr_cmd);
			get_redir(curr_tkn, curr_cmd);
		}
		else if (curr_tkn->type == CTRL_OP && curr_tkn->ctrlop == PIPE)
		{
			curr_cmd->next = ft_calloc_protec(1, sizeof(t_cmd_line));
			curr_cmd = curr_cmd->next;
			curr_cmd->cmds = ft_calloc_protec(count_cmd_tkns(curr_tkn->next) + 1, sizeof(char *));
			i = 0;
		}
		curr_tkn = curr_tkn->next;
	}
	return (cmd_line_head);
} */

