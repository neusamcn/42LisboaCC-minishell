/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:39:39 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 17:14:14 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"
#include "../../include/execution.h"

t_token	*xpnd_word_tkn(t_token *t, t_token **head, t_shelly *shelly)
{
	char	*xpndd_word;

	if (t->previous && t->previous->type == REDIR
		&& t->previous->redir == HEREDOC)
		return (t->next);
	xpndd_word = word_param_expansion(t->value, shelly);
	if (!xpndd_word)
	{
		t->word_xpndd = -1;
		return (t->next);
	}
	else if (xpndd_word[0] == 0 && t->word == CMD && t->next)
	{
		free(xpndd_word);
		if (*head == t)
			*head = t->next;
		return (rm_empty_xpnsn(t));
	}
	else
	{
		free(t->value);
		t->value = xpndd_word;
		t->word_xpndd = 1;
		return (t->next);
	}
}

t_token	*expand_params(t_token *tokens, t_shelly *shelly)
{
	t_token	*t;
	t_token	*head;

	if (!tokens)
		return (NULL);
	t = tokens;
	head = tokens;
	while (t)
	{
		if (t->word == CMD || t->word == QMARK2 || t->word == QMARK1)
			t = xpnd_word_tkn(t, &head, shelly);
		else
			t = t->next;
	}
	return (head);
}
