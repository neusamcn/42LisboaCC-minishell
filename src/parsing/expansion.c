/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 20:06:59 by megi              #+#    #+#             */
/*   Updated: 2026/05/23 21:36:16 by megiazar         ###   ########.fr       */
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

int xpnd_var(char *tkn_val, int i, char **xpndd_word, t_shelly *shelly)
{
	int 	start;
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

char *word_param_expansion(char *tkn_val, t_shelly *shelly)
{
	char	*xpndd_word;
	int		i;
	bool	in_single;
	bool	in_double;

	if (!tkn_val)
		return (ft_strdup(""));
	xpndd_word = ft_strdup("");
	if (!xpndd_word)
		return (NULL);
	i = 0;
	in_single = false;
	in_double = false;
	while (tkn_val[i])
	{
		if (in_single)
			handle_in_single(tkn_val, &i, &xpndd_word, &in_single);
		else if (in_double)
			handle_in_double(tkn_val, &i, &xpndd_word, shelly, &in_double);
		else
			handle_unquoted(tkn_val, &i, &xpndd_word, shelly,
				&in_single, &in_double);
	}
	return (xpndd_word);
}

void expand_params(t_token *t, t_shelly *shelly)
{
	char  *xpndd_word;

	while (t)
	{
		if (t->word == CMD || t->word == QMARK2 || t->word == QMARK1)
	{
		if (t->previous && t->previous->type == REDIR 
				&& t->previous->redir == HEREDOC)
		{
			printf("eh %s\n", t->value);
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

/* static int	xpnd_var(char *tkn_val, int i, char **xpndd_word, t_shelly *shelly)
{
	int		start;
	char	*envp_key;
	char	*envp_val;

	i++;
	if (tkn_val[i] == '?' && tkn_val[i - 1] == '$')
	{
		// TODO: expand to exit status of most recently executed foreground pipeline -- good?
		envp_val = ft_itoa(g_signal_stat);
		// ft_printf("envp_val signal stat: %s\n", envp_val);
		*xpndd_word = ft_strjoin_free(*xpndd_word, envp_val);
		// ft_printf("xpndd_word signal stat: %s\n", *xpndd_word);
		free(envp_val);
		return (i + 1);
	}
	start = i;
	while (is_varkey(tkn_val, i) == true)
		i++;
	envp_key = ft_substr(tkn_val, start, i - start);
	// ft_printf("envp_key: %s\n", envp_key);
	envp_val = find_var_shellyenvp(shelly, envp_key);
	if (!envp_val)
		envp_val = "";
	// ft_printf("envp_val: %s\n", envp_val);
	*xpndd_word = ft_strjoin_free(*xpndd_word, envp_val);
	// ft_printf("xpndd_word envp: %s\n", *xpndd_word);
	free(envp_key);
	return (i);
} */

/* static char	*word_param_expansion(char *tkn_val, t_shelly *shelly)
{
	char	*xpndd_word;
	int		i;

	xpndd_word = ft_strdup("");
	if (!xpndd_word)
		return (NULL);
	i = 0;
	while (tkn_val[i])
	{
		if (is_varkey(tkn_val, i) == true)
		{
			i = xpnd_var(tkn_val, i, &xpndd_word, shelly);
			// ft_printf("xpndd_word xpandvar: %s\n", xpndd_word);
			continue ;
		}
		i = cpy_norm_str(tkn_val, i, &xpndd_word);
		// ft_printf("xpndd_word cpy_norm_str: %s\n", xpndd_word);
	}
	return (xpndd_word);
} */
/* 

void	expand_params(t_token *tokens, t_shelly *shelly)
{
	char	*xpndd_word;

	if (!tokens || !shelly)
		return ;
	while (tokens)
	{
		if (tokens->word == CMD || tokens->word == QMARK2)
		{
			if (tokens->word == QMARK2)
			{
				xpndd_word = ft_strtrim(tokens->value, "\"");
				if (!xpndd_word)
				{
					tokens->word_xpndd = -1;
					return ; // TODO: print_err_msg() instead?
				}
				free(tokens->value);
				tokens->value = xpndd_word;
			}
			xpndd_word = word_param_expansion(tokens->value, shelly);
			if (!xpndd_word)
				tokens->word_xpndd = -1; // TODO: and return or err_msg?
			else
			{
				free(tokens->value);
				tokens->value = xpndd_word;
				tokens->word_xpndd = 1;
			}
		}
		else if (tokens->word == QMARK1)
		{
			xpndd_word = ft_strtrim(tokens->value, "'");
			if (!xpndd_word)
			{
				tokens->word_xpndd = -1;
				return ; // TODO: print_err_msg() instead?
			}
			free(tokens->value);
			tokens->value = xpndd_word;
			tokens->word_xpndd = 1;
		}
		tokens = tokens->next;
	}
}
 */