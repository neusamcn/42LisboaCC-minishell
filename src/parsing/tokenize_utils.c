/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 16:40:50 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/24 22:04:54 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

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

	while (input_str[i] && !ft_isspace(input_str[i])
		&& !char_is_op(input_str[i]))
	{
		if (input_str[i] == '\'' || input_str[i] == '"')
		{
			quote = input_str[i];
			i++;
			while (input_str[i] && input_str[i] != quote)
				i++;
			if (input_str[i] == quote)
				i++;
		}
		else
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

void	free_tkn(t_token *tkn)
{
	t_token	*next;

	while (tkn)
	{
		next = tkn->next;
		free(tkn->value);
		free(tkn);
		tkn = next;
	}
}
