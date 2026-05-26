/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 18:09:11 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 20:07:24 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	copy_money(char *tkn_val, int i, char **xpndd_word)
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

void	handle_qmark2(char *tkn_val, int *i, t_exp_state *st,
			t_shelly *shelly)
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
			*i = copy_money(tkn_val, *i, st->xpndd_word);
		return ;
	}
	else
	{
		buf[0] = tkn_val[*i];
		*st->xpndd_word = ft_strjoin_free(*st->xpndd_word, buf);
	}
	(*i)++;
}

void	handle_qmark1(char *tkn_val, int *i, t_exp_state *st)
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

void	handle_word_cmd(char *tkn_val, int *i, t_exp_state *st,
			t_shelly *shelly)
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
			*i = copy_money(tkn_val, *i, st->xpndd_word);
		return ;
	}
	*i = cpy_norm_str(tkn_val, *i, st->xpndd_word);
}
