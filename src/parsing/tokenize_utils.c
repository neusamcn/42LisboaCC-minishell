/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 16:40:50 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/19 18:08:08 by ncruz-ne         ###   ########.fr       */
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
		new_node->index = 0;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_node;
	new_node->previous = last;
	new_node->index = new_node->previous->index + 1;
}

int	scan_word_end(char *input_str, int i)
{
	char	quote;

	if (input_str[i] == '\'' || input_str[i] == '"')
	{
		quote = input_str[i];
		i++;
	}
	else
		quote = 0;
	while (input_str[i])
	{
		if (!quote && (ft_isspace(input_str[i]) == true
				|| char_is_op(input_str[i]) == true
				|| input_str[i] == '\'' || input_str[i] == '"'))
			break ;
		if (quote && input_str[i] == quote)
		{
			i++;
			break ;
		}
		i++;
	}
	return (i);
}

int	op_len(char *input_str)
{
	if (*input_str == input_str[1] && (*input_str == '<' || *input_str == '>'))
		return (2);
	if (*input_str == '|')
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
