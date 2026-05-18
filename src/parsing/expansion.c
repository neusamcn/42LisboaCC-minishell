/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 20:06:59 by megi              #+#    #+#             */
/*   Updated: 2026/05/18 16:56:09 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

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

/* typedef struct s_shelly
{
	char		**envp;
	t_cmd_line	*cur_cmd;
	int			fds_saved[2]; // put this ones as -1
	int			*open_fd; // Neusa, i am not using it, should delete?
	void		**malloc_ptrs;
}	t_shelly;
*/
/* typedef enum e_qstat
{
	NONE_Q;
	SINGLE_Q;
	TAKEN_Q;
} */

// in bash one backwards slash-> not execute
// one slash->execute with slash (?)

// УБРАТЬ: expand_single_quotes, expand_doub_quotes, старые bool not_to_be/to_be
// ОСТАВИТЬ И ПОЧИНИТЬ:

typedef struct e_expand
{
	char	*stror; // string original
	char	*strwor; // string after work
	int		i;
	int		flag;
	t_qstat place;
}	t_expand;

//void    not_to_be(t_qstat *state, int *i, char *word, char **result)
//adding char by char

void	not_to_be(t_expand *ex)
{
	char++;
	strwor[i] = '\0';
	
	ex->strwor = expand_dollar_sign(ex->strwor, shelly); // malloc here huuuhhuhsdinkmldz,
	while (ex->stror[ex->i] && ex->strwor)
	{
		if (ft_strchr(ex->strwor, "'") == 0)
			ex->i++;
		if (ft_strchr(ft_strlen(ex->strwor + 1), "'") == 0)
			break ;
	}
	return (ex->strwor);
}

/* '$USER' -> $USER
"'"'$USER'"'" -> 'megiazavr' */

// tokens: value->word
// quotes: have QMARK1, QMARK2

char	*to_be_or_not_to_be(t_expand *ex, t_shelly *shelly)
{
	ex->i = 0;
	ex->place = OUTSIDE_OF_QUOTES;
	while (ex->stror[ex->i])
	{
		if (ex->stror[ex->i] == TAKEN_Q && ex->place != INSIDE_SINGLE_QUOTES)
		{
			//ex->flag = TAKEN_Q;
			ex->place = INSIDE_TAKEN_QUOTES;
			ex->i++;
			while (ex->stror[ex->i] && ex->stror[ex->i] != ex->stror[ex->i] != TAKEN_Q)
			{
				if (ex->stror[ex->i] == '$')
					to_be();
				else
					not_to_be();
			}
			if (ex->stror[ex->i] == TAKEN_Q)
            {
                ex->place = OUTSIDE_OF_QUOTES;
                ex->i++;
            }
		}
		if (ex->stror[ex->i] == SINGLE_Q  && ex->place != INSIDE_TAKEN_QUOTES)
		{
			//ex->flag = SINGLE_Q;
			ex->place = INSIDE_SINGLE_QUOTES;
			ex->i++;
			while (ex->stror[ex->i] && ex->stror[ex->i] != ex->flag)
				not_to_be();
			if (ex->stror[ex->i] == SINGLE_Q)
            {
                ex->place = OUTSIDE_OF_QUOTES;
                ex->i++;
            }
		}
	}
    if (ex->stror[ex->i] == '$')
        to_be();
	else
        not_to_be();
}
			
void    to_be(t_qstat *state, int *i, char *word, char **result, t_shelly *shelly) // ← shelly параметр
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
		/* 
		char    *to_be_or_not_to_be(char *word, t_shelly *shelly)
		{
    char    *result;
    t_qstat place;
    int     i;

    result = ft_strdup("");
    place = OUTSIDE_OF_QUOTES;
    i = 0;
    while (word[i])
    {
        if ((word[i] == SINGLE_Q))
            {
				not_to_be(&place, &i, word, &result);
				place = ;
        else if (word[i] == TAKEN_Q && place == INSIDE_TAKEN_QUOTES)
            to_be(&place, &i, word, &result, shelly);
        else if (word[i] == TAKEN_Q)
            result = ft_strjoin_free(result, expand_dollar_sign(word, &i, shelly));
        else if (word[i] == '$' && place == INSIDE_TAKEN_QUOTES)
		{
			to_be();
		}
		else
			{
				char tmp[2] = {word[i], '\0'};
				result = ft_strjoin_free(result, ft_strdup(tmp));
				i++;
			}
    }
    return (result);
}
} */
/* word[i] = '\'' ;
word[i] = SINGLE_Q ; */

/* bool not_to_be()
{
	not_expand, just to print whatever inside the quotes
}

bool to_be()
{
	expand, and if it is "'"$USER"'" in this case it will expand, but with 'megiazar'
}
 */

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
