/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 16:40:50 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/16 16:45:54 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// TODO: review and test file

t_token	*new_tkn(t_token_type type, char *value)
{
	t_token	*token;

	token = ft_calloc_protec(1, sizeof(t_token));
	token->type = type;
	token->value = value;
	return (token);
}

void	append_tkn(t_token **head, t_token *new_node)
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

int	scan_word_end(char *s, int i)
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

int	op_len(char *s)
{
	if ((s[0] == '<' && s[1] == '<') || (s[0] == '>' && s[1] == '>'))
		return (2);
	if (s[0] == '|')
		return (1);
	return (1);
}

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
