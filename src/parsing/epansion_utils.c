/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epansion_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 18:09:11 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/24 05:36:03 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	cpy_dollar_literal(char *tkn_val, int i, char **xpndd_word)
{
	char	buf[2];

	buf[1] = '\0';
	buf[0] = tkn_val[i];
	*xpndd_word = ft_strjoin_free(*xpndd_word, buf);
		i++;
	while (ft_isalnum(tkn_val[i]) || tkn_val[i] == '_'
		|| tkn_val[i] == '?')
	{
		buf[0] = tkn_val[i];
		*xpndd_word = ft_strjoin_free(*xpndd_word, buf);
		i++;
	}
	return (i);
}

void	handle_in_double(char *tkn_val, int *i, t_exp_state *st, t_shelly *shelly)
{
	char	buf[2];

	buf[1] = '\0';
	if (tkn_val[*i] == '"')
		st->in_double = false;
	else if (tkn_val[*i] == '$' && tkn_val[*i + 1]
		&& (ft_isalnum(tkn_val[*i + 1]) || tkn_val[*i + 1] == '_'
		|| tkn_val[*i + 1] == '?'))
		{
		if (shelly)
			*i = xpnd_var(tkn_val, *i, st->xpndd_word, shelly);
		else
			*i = cpy_dollar_literal(tkn_val, *i, st->xpndd_word);
		return ;
	}
	else
	{
		buf[0] = tkn_val[*i];
		*st->xpndd_word = ft_strjoin_free(*st->xpndd_word, buf);
	}
	(*i)++;
}

void	handle_in_single(char *tkn_val, int *i, t_exp_state *st)
{
	char	buf[2];

	buf[1] = '\0';
	if (tkn_val[*i] == '\'')
		st->in_single = false;
	else
	{
		buf[0] = tkn_val[*i];
		*st->xpndd_word = ft_strjoin_free(*st->xpndd_word, buf);
	}
	(*i)++;
}

void	handle_unquoted(char *tkn_val, int *i, t_exp_state *st, t_shelly *shelly)
{
	if (tkn_val[*i] == '\'')
		return (st->in_single = true, (void)(*i)++);
	if (tkn_val[*i] == '"')
		return (st->in_double = true, (void)(*i)++);
	if (tkn_val[*i] == '$' && tkn_val[*i + 1]
		&& (ft_isalnum(tkn_val[*i + 1]) || tkn_val[*i + 1] == '_'
		|| tkn_val[*i + 1] == '?'))
	{
		if (shelly)
			*i = xpnd_var(tkn_val, *i, st->xpndd_word, shelly);
		else
			*i = cpy_dollar_literal(tkn_val, *i, st->xpndd_word);
		return ;
	}
	*i = cpy_norm_str(tkn_val, *i, st->xpndd_word);
}
