/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 20:06:59 by megi              #+#    #+#             */
/*   Updated: 2026/05/24 16:40:21 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static bool	is_varkey(char *str, int i)
{
	int	j;

	if (!str || !str[i])
		return (false);
	if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1])
			|| str[i + 1] == '_' || str[i + 1] == '?'))
		return (true);
	if (i > 0 && str[i - 1] == '$' && (ft_isalnum(str[i])
			|| str[i] == '_' || str[i] == '?'))
		return (true);
	if (ft_isalnum(str[i]) || str[i] == '_')
	{
		j = 1;
		while (i - j >= 0 && str[i - j])
		{
			if (str[i - j] == '$')
				return (true);
			else if (ft_isalnum(str[i - j]) || str[i - j] == '_')
				j++;
			else
				break ;
		}
	}
	return (false);
}

int	cpy_norm_str(char *tkn_val, int i, char **xpndd_word)
{
	int		start;
	char	*tmp;

	start = i;
	while (tkn_val[i] && is_varkey(tkn_val, i) == false
			&& tkn_val[i] != '\'' && tkn_val[i] != '"')
		i++;
	tmp = ft_substr(tkn_val, start, i - start);
	*xpndd_word = ft_strjoin_free(*xpndd_word, tmp);
	free(tmp);
	return (i);
}

int	xpnd_var(char *tkn_val, int i, char **xpndd_word, t_shelly *shelly)
{
	int		start;
	char	*envp_key;
	char	*envp_val;

	i++;
	if (tkn_val[i] == '?' && tkn_val[i - 1] == '$')
	{
		envp_val = ft_itoa(g_signal_stat);
		*xpndd_word = ft_strjoin_free(*xpndd_word, envp_val);
		free(envp_val);
		return (i + 1);
	}
	start = i;
	while (ft_isalnum(tkn_val[i]) || tkn_val[i] == '_')
		i++;
	envp_key = ft_substr(tkn_val, start, i - start);
	envp_val = find_var_shellyenvp(shelly, envp_key);
	if (!envp_val)
		envp_val = "";
	*xpndd_word = ft_strjoin_free(*xpndd_word, envp_val);
	free(envp_key);
	return (i);
}

char	*word_param_expansion(char *tkn_val, t_shelly *shelly)
{
	t_exp_state	st;
	int			i;

	if (!tkn_val)
		return (ft_strdup(""));
	st.xpndd_word = &(char *){ft_strdup("")};
	st.in_single = false;
	st.in_double = false;
	i = 0;
	while (tkn_val[i])
	{
		if (st.in_single)
			handle_in_single(tkn_val, &i, &st);
		else if (st.in_double)
			handle_in_double(tkn_val, &i, &st, shelly);
		else
			handle_unquoted(tkn_val, &i, &st, shelly);
	}
	return (*st.xpndd_word);
}

void	expand_params(t_token *t, t_shelly *shelly)
{
	char	*xpndd_word;

	while (t)
	{
		if (t->word == CMD || t->word == QMARK2 || t->word == QMARK1)
		{
			if (t->previous && t->previous->type == REDIR
					&& t->previous->redir == HEREDOC)
			{
				t = t->next;
				continue ;
			}
			xpndd_word = word_param_expansion(t->value, shelly);
			if (!xpndd_word)
				t->word_xpndd = -1;
			else
			{
				free(t->value);
				t->value = xpndd_word;
				t->word_xpndd = 1;
			}
		}
		t = t->next;
	}
}
