/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 20:06:59 by megi              #+#    #+#             */
/*   Updated: 2026/05/12 14:01:50 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
	t_token	*tokenize_input(char *input_str)
*/

/* 
	parser() {
		1. tokenise
			- token is variable depending on quote state and variable name being valid 
			E.G. $?, $_, $<alphanumeric>, $! check subject 
		2. sytax check
		3. convert to command pipeline 
			- expand if variable
				- token->val: $var
				- token->exp = true/false
				- token->quoted = true/false
	}

	1. Happens building command pipeline. Expand the token to when converting to the cmd pipeline
		export var = "Milena"
		echo "$var" -> Milena -> exp = true
		echo '$var' -> $var -> exp = false
		'joh"n"' -> joh"n"

	2. Heredoc after command pipeline is built / during execution
		cat << del
		> $var
		> del
		Milena
		
		cat << del
		> "$var"
		> del
		"Milena"

		cat << "del" / ""del / del"" / "d"el
		> $var
		> del
		$var
*/


/* $VAR → replace with env value
$? → replace with last exit status
~ → replace with HOME (optional/bonus)
"$VAR" → expand inside double quotes
'$VAR' → do NOT expand inside single quotes */

/* "hello $USER world"
		 ↓
scan char by char:
  - regular char → copy as-is
  - ' → copy everything until next ' literally
  - " → copy until next " BUT expand $ inside
  - $ → look ahead, grab var name, look up in envp, substitute */

char    *expand_dollar_sign(char *w, t_shelly *shelly)
{
	char *str;
	int index;
	char	*copied_w;
	char		*var;

	copied_w = ft_strdup("");
	index = 0;
	while (str[index])
	{
		if (ft_strcmp(w[0], "$") == 0)
		{
			index++;
			if (ft_isalnum(w[index]) || w[index] == "_")
				ft_substr(w, index, ft_strlen(copied_w));
			index++;
		}
		//w[index] = '\0';
	}
	return (copied_w);
}

char	*expand_single_quotes(char *word, t_shelly (*shelly))
{
	int		ind;
	char	*str;

	ind = 0;
	word = expand_dollar_sign(word, shelly);
	while (str[ind] && word)
	{
		if (ft_strchr(word[ind - 1], "'") == 0)
			ind++;
		if (ft_strchr(ft_strlen(word + 1), "'") == 0)
			break ;
	}
	return (word);
}

char	*expand_doub_quotes(char *w, t_shelly *shelly)
{
	char	*str;
	int		ind;
	int		find_the_pair;
	int		whole_w;

	ind = 0;
	w = expand_dollar_sign(w, shelly);
	whole_w = ft_strlen(w[ind]);
	while (str[ind])
	{
		find_the_pair = 0;
		if (ft_strchr(w[0], '"') == 0)
			ind++;
		if (ft_strcmp(w[whole_w], '"') == 0)
			break ; 
	}
	return (w);
}