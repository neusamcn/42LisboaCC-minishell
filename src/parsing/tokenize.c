/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 21:02:43 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/11 00:13:53 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// TODO: review and test file

// static t_token_type	tkn_sym_type(char *input_str)
// {
// 	if (input_str == ">>" || input_str == "<<"
// 		|| input_str + 1 == ">" || input_str + 1 == "<")  // can I do this?
// 		return (REDIR);
// 	else if (input_str + 1 == "|")
// 		return (CTRL_OP);
// 	return (WORD);
// }

// static t_token_type	tkn_type_check(char *input_str)
// {
// 	// if (input_str == ">>" || input_str == "<<"
// 	// 	|| input_str + 1 == ">" || input_str + 1 == "<")
// 	// 	return (REDIR);
// 	// else if (input_str + 1 == "|") // can I do this?
// 	// 	return (CTRL_OP);
// 	// return (WORD);
// }

static t_token	*new_tkn(t_token_type type, char *value)
{
	t_token	*token;

	token = ft_calloc_protec(1, sizeof(t_token));
	token->type = type;
	token->value = value;
	return (token);
}

static void	append_tkn(t_token **head, t_token *new_node)
{
	t_token	*last;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_node;
	new_node->previous = last;
}

static t_token	*add_tkn_word(char *word, int word_len, t_token *tokens)
{
	t_token	*node;
	char	*value;

	value = ft_substr(word, 0, word_len);
	if (!value)
		return (tokens);
	node = new_tkn(WORD, value);
	if (ft_strchr(value, '\'') || ft_strchr(value, '"'))
		node->quoted = 1;
	append_tkn(&tokens, node);
	return (tokens);
}

static int	scan_word_end(char *s, int i)
{
	char	quote;

	quote = 0;
	while (s[i])
	{
		if (!quote && ft_isspace(s[i] == true))
			break ;
		if (!quote && (s[i] == '>' || s[i] == '<' || s[i] == '|'))
			break ;
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		i++;
	}
	return (i);
}

static int	op_len(char *s)
{
	if ((s[0] == '<' && s[1] == '<') || (s[0] == '>' && s[1] == '>'))
		return (2);
	if (s[0] == '|')
		return (1);
	return (1);
}

static t_token	*add_tkn_op(char *input, t_token *tokens)
{
	t_token	*node;
	int		len;

	len = op_len(input);
	node = ft_calloc_protec(1, sizeof(t_token));
	if (input[0] == '|')
		node->type = CTRL_OP;
	else
		node->type = REDIR;
	if (len == 2 && input[0] == '<')
		node->redir = HEREDOC;
	else if (len == 2 && input[0] == '>')
		node->redir = APPEND;
	else if (input[0] == '<')
		node->redir = IN;
	else if (input[0] == '>')
		node->redir = OUT;
	else
		node->ctrlop = PIPE;
	node->value = ft_substr(input, 0, len);
	append_tkn(&tokens, node);
	return (tokens);
}

// TODO: if (str[i] != '`' && str[i]) // 96 ==> add?
t_token	*tokenize_input(char *input_str)
{
	t_token	*tokens;
	int		i;
	int		start;
	int		len;

	tokens = NULL;
	i = 0;
	while (input_str[i])
	{
		while (input_str[i] && ft_isspace(input_str[i]) == true)
			i++;
		if (!input_str[i])
			break ;
		if (input_str[i] == '|' || input_str[i] == '>' || input_str[i] == '<')
		{
			tokens = add_tkn_op(input_str + i, tokens);
			i += op_len(input_str + i);
		}
		else
		{
			start = i;
			i = scan_word_end(input_str, i);
			len = i - start;
			tokens = add_tkn_word(input_str + start, len, tokens);
		}
	}
	return (tokens);
}


/*
1) Step-by-step tokenizer algorithm
Initialize i = 0, empty token list.
Skip spaces/tabs.
If current char starts an operator:
Match 2-char first: <<, >> (and &&, || if bonus).
Else match 1-char: |, <, >.
Push operator token, advance index.
Else parse a word:
Start buffer.
While not end, not unquoted whitespace, and not unquoted operator:
If ' → copy until next ' literally (error if missing closing).
If " → copy until next " with shell rules (allow escapes if you support them).
If \ outside single quotes → escape next char.
Else copy char.
Push WORD token.
Repeat until end.
Optional syntax validation pass:
no leading/trailing pipe
no ||/&& (if unsupported)
redirections followed by a WORD
Return token list or error.
*/