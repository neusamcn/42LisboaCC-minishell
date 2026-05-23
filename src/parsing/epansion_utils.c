/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epansion_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 18:09:11 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/23 21:28:19 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int cpy_dollar_literal(char *tkn_val, int i, char **xpndd_word)
{
	char buf[2];

	buf[1] = '\0';
	buf[0] = tkn_val[i];
	printf("dollar: %s i = %d\n", tkn_val, i);
	*xpndd_word = ft_strjoin_free(*xpndd_word, buf);
	i++;
	while (ft_isalnum(tkn_val[i]) || tkn_val[i] == '_' || tkn_val[i] == '?')
	{
		buf[0] = tkn_val[i];
		*xpndd_word = ft_strjoin_free(*xpndd_word, buf);
		i++;
	}
	return (i);
}

void handle_in_double(char *tkn_val, int *i, char **xpndd_word,
	t_shelly *shelly, bool *in_double)
{
	char buf[2];

	buf[1] = '\0';
	if (tkn_val[*i] == '"')
		*in_double = false;
	else if (tkn_val[*i] == '$' && tkn_val[*i + 1]
			&& (ft_isalnum(tkn_val[*i + 1]) || tkn_val[*i + 1] == '_'
			|| tkn_val[*i + 1] == '?'))
	{
		if (shelly)
			*i = xpnd_var(tkn_val, *i, xpndd_word, shelly);
		else
			*i = cpy_dollar_literal(tkn_val, *i, xpndd_word);
		return ;
	}
	else
	{
		buf[0] = tkn_val[*i];
		*xpndd_word = ft_strjoin_free(*xpndd_word, buf);
	}
	(*i)++;
}

void	handle_in_single(char *tkn_val, int *i, char **xpndd_word,
	bool *in_single)
{
	char buf[2];

	buf[1] = '\0';
	if (tkn_val[*i] == '\'')
		*in_single = false;
	else
	{
		buf[0] = tkn_val[*i];
		*xpndd_word = ft_strjoin_free(*xpndd_word, buf);
	}
		(*i)++;
}

void	handle_unquoted(char *tkn_val, int *i, char **xpndd_word,
	t_shelly *shelly, bool *in_single, bool *in_double)
{
	if (tkn_val[*i] == '\'')
	{
		*in_single = true;
		(*i)++;
	}
	else if (tkn_val[*i] == '"')
	{
		*in_double = true;
		(*i)++;
	}
	else if (tkn_val[*i] == '$' && tkn_val[*i + 1]
			&& (ft_isalnum(tkn_val[*i + 1]) || tkn_val[*i + 1] == '_'
			|| tkn_val[*i + 1] == '?'))
	{
		if (shelly)
			*i = xpnd_var(tkn_val, *i, xpndd_word, shelly);
		else
			*i = cpy_dollar_literal(tkn_val, *i, xpndd_word);
		return ;
	}
	else
		*i = cpy_norm_str(tkn_val, *i, xpndd_word);
}
