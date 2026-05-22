/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 21:02:43 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/22 19:57:37 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// TODO: review and test file

static t_token	*add_tkn_word(char *word, int len, t_token *tkns, bool space_b4)
{
	t_token	*tkn_nd;
	char	*value;

	value = ft_substr(word, 0, len);
	if (!value)
		return (tkns);
	tkn_nd = new_tkn(WORD, value);
	if (ft_strchr(value, '\''))
		tkn_nd->word = QMARK1;
	else if (ft_strchr(value, '"'))
		tkn_nd->word = QMARK2;
	else
		tkn_nd->word = CMD;
	tkn_nd->space_b4_word = space_b4;
	append_tkn(&tkns, tkn_nd);
	return (tkns);
}

static void	set_op_type(char *input_str, t_token *tkn_nd, int len)
{
	if (len == 1 && *input_str == '|')
	{
		tkn_nd->type = CTRL_OP;
		tkn_nd->ctrlop = PIPE;
	}
	else
	{
		tkn_nd->type = REDIR;
		if (len == 1)
		{
			if (*input_str == '<')
				tkn_nd->redir = IN;
			else if (*input_str == '>')
				tkn_nd->redir = OUT;
		}
		else if (len == 2)
		{
			if (ft_strncmp(input_str, ">>", 2) == 0)
				tkn_nd->redir = APPEND;
			else if (ft_strncmp(input_str, "<<", 2) == 0)
				tkn_nd->redir = HEREDOC;
		}
	}
}

static t_token	*add_tkn_op(char *input_str, t_token *tokens)
{
	t_token	*tkn_nd;
	int		len;

	len = op_len(input_str);
	tkn_nd = ft_calloc_protec(1, sizeof(t_token));
	set_op_type(input_str, tkn_nd, len);
	tkn_nd->value = ft_substr(input_str, 0, len);
	append_tkn(&tokens, tkn_nd);
	return (tokens);
}

static int	add_tkn(char *input_str, int i, t_token **tokens)
{
	int		start;
	int		len;
	bool	space_b4_word;

	space_b4_word = false;
	if (char_is_op(input_str[i]) == true)
	{
		*tokens = add_tkn_op(input_str + i, *tokens);
		i += op_len(input_str + i);
	}
	else
	{
		start = i;
		i = scan_word_end(input_str, i);
		len = i - start;
		if (input_str[i - 1])
			space_b4_word = ft_isspace(input_str[i - 1]);
		*tokens = add_tkn_word(input_str + start, len, *tokens, space_b4_word);
	}
	return (i);
}

t_token	*tokenize_input(char *input_str)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input_str[i])
	{
		while (input_str[i] && ft_isspace(input_str[i]) == true)
			i++;
		if (!input_str[i])
			break ;
		i = add_tkn(input_str, i, &tokens);
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