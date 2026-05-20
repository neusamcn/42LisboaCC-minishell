/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 20:06:59 by megi              #+#    #+#             */
/*   Updated: 2026/05/20 01:49:19 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

/* static bool	char_is_varkey(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '?')
		return (true);
	return (false);
} */

static bool	is_var_key(char c)
{
    return (ft_isalnum(c) || c == '_' || c == '?');
}

static char	*join_free(char *left, const char *right)
{
    char	*joined;

    if (!right)
        return (left);
    if (!left)
        left = ft_strdup("");
    if (!left)
        return (NULL);
    joined = ft_strjoin(left, right);
    free(left);
    return (joined);
}

static char	*append_char(char *str, char c)
{
    char	buf[2];

    buf[0] = c;
    buf[1] = '\0';
    return (join_free(str, buf));
}

static char	*expand_one_token(char *value, t_shelly *shelly)
{
    char	*result;
    int		i;
    int		start;
    char	*key;
    char	*env_value;

    result = ft_strdup("");
    if (!result)
        return (NULL);
    i = 0;
    while (value[i])
    {
        if (value[i] == '$' && value[i + 1] && is_var_key(value[i + 1]))
        {
            i++;
            if (value[i] == '?')
            {
                env_value = ft_itoa(g_signal_stat);
                result = join_free(result, env_value);
                free(env_value);
                i++;
                continue ;
            }
            start = i;
            while (value[i] && is_var_key(value[i]))
                i++;
            key = ft_substr(value, start, i - start);
            env_value = find_var_shellyenvp(shelly, key);
            result = join_free(result, env_value);
            free(key);
            continue ;
        }
        result = append_char(result, value[i]);
        i++;
    }
    return (result);
}

void	expand_params(t_token *tokens, t_shelly *shelly)
{
    char	*expanded;

    if (!tokens || !shelly)
        return ;
    while (tokens)
    {
        if (tokens->word == CMD || tokens->word == QMARK2)
        {
            expanded = expand_one_token(tokens->value, shelly);
            if (expanded)
            {
                free(tokens->value);
                tokens->value = expanded;
                tokens->word_xpnd = 1;
            }
            else
                tokens->word_xpnd = -1;
        }
        tokens = tokens->next;
    }
}

/* static char	*append2str(char *old, char const *buf)
{
	char	*new;
	size_t	i_new;
	size_t	i_buf;

	if (!buf || !*buf)
		return (old);
	new = ft_calloc(ft_strlen(old) + ft_strlen(buf) + 2, 1);
	if (!new)
	{
		free(old);
		return (NULL);
	}
	i_new = 0;
	while (old && old[i_new])
	{
		new[i_new] = old[i_new];
		i_new++;
	}
	free(old);
	i_buf = 0;
	while (buf[i_buf])
		new[i_new++] = buf[i_buf++];
	return (new);
}

static int	cpy_norm_str(char *og, int i, char **dest)
{
	int	col;
	int	start;

	col = 0;
	start = i;
	while (og[i] && og[i] != '$' && char_is_varkey(og[i + 1]) == false)
	{
		col++;
		i++;
	}
	*dest = ft_substr(og, start, col);
	return (i);
}

static int	xpnd_var(char *tkn_val, int i, char **row, t_shelly *shelly)
{
	int		col;
	char	*envp_key;

	i++;
	if (tkn_val[i] == '?')
	{
		// TODO: expand to exit status of most recently executed foreground pipeline -- good?
		*row = ft_itoa(g_signal_stat);
		i++;
		return (i);
	}
	col = 0;
	while (tkn_val[i + col] && char_is_varkey(tkn_val[i + col]) == true)
		col++;
	envp_key = ft_substr(tkn_val, i, col);
	// *row = ft_calloc_protec(1, sizeof(char *));
	*row = find_var_shellyenvp(shelly, envp_key);
	free(envp_key);
	envp_key = NULL;
	i += col;
	return (i);
}

static void	word_param_expansion(t_token *tkn, t_shelly *shelly)
{
	int		i;
	int		row;
	char	**tkn_val_mtrx;
	char	*expanded_word;
	// char	*envp_key;
	// int		col;

	// TODO: remove if not necessary
	// if (tkn->word != CMD || tkn->word != QMARK2)
	// 	tkn->word_xpnd = -1;
	tkn_val_mtrx = ft_calloc_protec(100, sizeof(char *));
	row = 0;
	i = 0;
	while (tkn->value[i])
	{
		if (tkn->value[i] != '$' && char_is_varkey(tkn->value[i + 1]) == false)
		{
			i = cpy_norm_str(tkn->value, i, &tkn_val_mtrx[row]);
			row++;
			continue ;
			// col = 0;
			// while (tkn->value[i] && tkn->value[i] != '$' && char_is_varkey(tkn->value[i + 1]) == false)
			// 	col++;
			// tkn_val_mtrx[row++] = ft_substr(tkn->value, i, col);
			// i += col;
		}
		if (tkn->value[i] == '$' && char_is_varkey(tkn->value[i + 1]) == true)
		{
			i = xpnd_var(tkn->value, i, &tkn_val_mtrx[row], shelly);
			row++;
			continue ;
			// i++;
			// if (tkn->value[i] == '?')
			// {
			// 	// TODO: expand to exit status of most recently executed foreground pipeline -- good?
			// 	tkn_val_mtrx[row++] = ft_itoa(g_signal_stat);
			// 	i++;
			// 	continue ;
			// }
			// col = 0;
			// while (tkn->value[col] && char_is_varkey(tkn->value[col]) == true)
			// 	col++;
			// envp_key = ft_substr(tkn->value, i, col);
			// tkn_val_mtrx[row] = ft_calloc_protec(1, sizeof(char *));
			// tkn_val_mtrx[row++] = find_var_shellyenvp(shelly, envp_key);
			// free(envp_key);
			// envp_key = NULL;
			// i += col;
		}
		else
			i++; // TODO: needed?
	}
	tkn_val_mtrx[row] = NULL;
	row = 0;
	if (tkn_val_mtrx[row])
		expanded_word = ft_strdup(tkn_val_mtrx[row]);
	free(tkn_val_mtrx[row]);
	while (tkn_val_mtrx[++row])
	{
		expanded_word = append2str(expanded_word, tkn_val_mtrx[row]);
		free(tkn_val_mtrx[row]);
	}
	free(tkn_val_mtrx);
	if (tkn->word == QMARK2)
		tkn->value = ft_strtrim(expanded_word, "\"");
	else
		tkn->value = expanded_word;
	tkn->word_xpnd = 1;
}


void	expand_params(t_token *tokens, t_shelly *shelly)
{
	if (!tokens || !shelly)
		return ;
	while (tokens)
	{
		if (tokens->word == CMD || tokens->word == QMARK2)
			word_param_expansion(tokens, shelly);
		tokens = tokens->next;
	}
}
 */


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

// typedef struct e_expand
// {
// 	char	*stror; // string original
// 	char	*strwor; // string after work
// 	int		i;
// 	int		flag;
// 	t_qstat place;
// }	t_expand;

// //void    not_to_be(t_qstat *state, int *i, char *word, char **result)
// //adding char by char

// void	not_to_be(t_expand *ex)
// {
// 	char++;
// 	strwor[i] = '\0';
	
// 	ex->strwor = expand_dollar_sign(ex->strwor, shelly); // malloc here huuuhhuhsdinkmldz,
// 	while (ex->stror[ex->i] && ex->strwor)
// 	{
// 		if (ft_strchr(ex->strwor, "'") == 0)
// 			ex->i++;
// 		if (ft_strchr(ft_strlen(ex->strwor + 1), "'") == 0)
// 			break ;
// 	}
// 	return (ex->strwor);
// }

// /* '$USER' -> $USER
// "'"'$USER'"'" -> 'megiazavr' */

// // tokens: value->word
// // quotes: have QMARK1, QMARK2

// char	*to_be_or_not_to_be(t_expand *ex, t_shelly *shelly)
// {
// 	ex->i = 0;
// 	ex->place = OUTSIDE_OF_QUOTES;
// 	while (ex->stror[ex->i])
// 	{
// 		if (ex->stror[ex->i] == TAKEN_Q && ex->place != INSIDE_SINGLE_QUOTES)
// 		{
// 			//ex->flag = TAKEN_Q;
// 			ex->place = INSIDE_TAKEN_QUOTES;
// 			ex->i++;
// 			while (ex->stror[ex->i] && ex->stror[ex->i] != ex->stror[ex->i] != TAKEN_Q)
// 			{
// 				if (ex->stror[ex->i] == '$')
// 					to_be();
// 				else
// 					not_to_be();
// 			}
// 			if (ex->stror[ex->i] == TAKEN_Q)
//             {
//                 ex->place = OUTSIDE_OF_QUOTES;
//                 ex->i++;
//             }
// 		}
// 		if (ex->stror[ex->i] == SINGLE_Q  && ex->place != INSIDE_TAKEN_QUOTES)
// 		{
// 			//ex->flag = SINGLE_Q;
// 			ex->place = INSIDE_SINGLE_QUOTES;
// 			ex->i++;
// 			while (ex->stror[ex->i] && ex->stror[ex->i] != ex->flag)
// 				not_to_be();
// 			if (ex->stror[ex->i] == SINGLE_Q)
//             {
//                 ex->place = OUTSIDE_OF_QUOTES;
//                 ex->i++;
//             }
// 		}
// 	}
//     if (ex->stror[ex->i] == '$')
//         to_be();
// 	else
//         not_to_be();
// }
			
// void    to_be(t_qstat *state, int *i, char *word, char **result, t_shelly *shelly) // ← shelly параметр
// {
// 	char	*str;
// 	int		ind;
// 	int		find_the_pair;
// 	int		whole_w;

// 	ind = 0;
// 	w = expand_dollar_sign(w, shelly);
// 	whole_w = ft_strlen(w[ind]);
// 	while (str[ind])
// 	{
// 		find_the_pair = 0;
// 		if (ft_strchr(w[0], '"') == 0)
// 			ind++;
// 		if (ft_strcmp(w[whole_w], '"') == 0)
// 			break ; 
// 	}
// 	return (w);
// }
// 		/* 
// 		char    *to_be_or_not_to_be(char *word, t_shelly *shelly)
// 		{
//     char    *result;
//     t_qstat place;
//     int     i;

//     result = ft_strdup("");
//     place = OUTSIDE_OF_QUOTES;
//     i = 0;
//     while (word[i])
//     {
//         if ((word[i] == SINGLE_Q))
//             {
// 				not_to_be(&place, &i, word, &result);
// 				place = ;
//         else if (word[i] == TAKEN_Q && place == INSIDE_TAKEN_QUOTES)
//             to_be(&place, &i, word, &result, shelly);
//         else if (word[i] == TAKEN_Q)
//             result = ft_strjoin_free(result, expand_dollar_sign(word, &i, shelly));
//         else if (word[i] == '$' && place == INSIDE_TAKEN_QUOTES)
// 		{
// 			to_be();
// 		}
// 		else
// 			{
// 				char tmp[2] = {word[i], '\0'};
// 				result = ft_strjoin_free(result, ft_strdup(tmp));
// 				i++;
// 			}
//     }
//     return (result);
// }
// } */
// /* word[i] = '\'' ;
// word[i] = SINGLE_Q ; */

// /* bool not_to_be()
// {
// 	not_expand, just to print whatever inside the quotes
// }

// bool to_be()
// {
// 	expand, and if it is "'"$USER"'" in this case it will expand, but with 'megiazar'
// }
//  */

// char    *expand_dollar_sign(char *w, t_shelly *shelly)
// {
// 	char *str;
// 	int index;
// 	char	*copied_w;
// 	char		*var;

// 	copied_w = ft_strdup("");
// 	index = 0;
// 	while (str[index])
// 	{
// 		if (ft_strcmp(w[0], "$") == 0)
// 		{
// 			index++;
// 			if (ft_isalnum(w[index]) || w[index] == "_")
// 				ft_substr(w, index, ft_strlen(copied_w));
// 			index++;
// 		}
// 		//w[index] = '\0';
// 	}
// 	return (copied_w);
// }
