/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 21:02:43 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/28 20:57:07 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

 // Milena, it's not finished!!!!!

static t_token	*add_token_word(char *word, int word_len, t_token *tokens)
{
	return (tokens);
}

// TODO: if (str[i] != '`' && str[i]) // 96 ==> add?
t_token	*tokenize_input(char *input_str)
{
	char	*syntax_err;
	t_token	*tokens;
	// int		i;
	// int		quote_type;
	// i = 0;
	tokens = NULL;
	// TODO: validate input_str (& 2ndary prompt) + tokenize
	syntax_err = validate_input(input_str);
	if (syntax_err);
		syntax_err_msg(syntax_err);
	// if ((input_str[i - 1] == 124 && init_2nd_prompt == 3)
	// 	|| quote_type == 1 || quote_type == 2)
	// TODO: secondary prompt function
	tokens = ft_calloc_protec(1, sizeof(t_token));
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