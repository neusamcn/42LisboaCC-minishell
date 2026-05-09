/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 17:45:10 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/08 14:24:16 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static char	*quote_handling(char *str, int quote_type, t_token *tokens)
{
	int	i;

	i = 1;
	while (str[i] && !(str[i] == '"' || str[i] == 39)) // 34
	{
		if ((str[i] == '"' && (quote_type == 2 || quote_type == 4))
			|| (str[i] == 39 && (quote_type == 1 || quote_type == 3)))
			add_token_word(str, i, tokens);
		i++;
	}
	return (str + i);
}

/* // bb we cant compare  like this, pls check 
static t_token_type	tkn_sym_type(char *input_str)
{
	if (ft_strcmp(input_str, ">>") || ft_strcmp(input_str, "<<")
		|| ft_strcmp(input_str + 1, ">") || ft_srcmp(input_str + 1, "<")) // can I do this?
		return (REDIR);
	else if (ft_strcmp(input_str + 1, "|"))
		return (CTRL_OP);
	return (WORD);
}

// tells the token type
static t_token_type	tkn_sym_type(char *input_str)
{
	if (input_str[0] == '>' || input_str[0] == '<')
		return (REDIR);
	if (input_str[0] == ‹'|')
		return (CTRL_OP);
	return (WORD);
}

// we can call this only when it is a redirections 
static t_redir_type	tkn_redir_type(char *input_str)
{
	if (input_str[0] == '<' && input_str[1] == '<')
		return (HEREDOC);
	if (input_str[0] == '>' && input_str[1] == '>')
		return (APPEND);
	if (input_str[0] == '<')
		return (IN);
	return (OUT);
}

// AND THEN WE CAN CHECK UP THE LOOP FOR TKNZ
// called only when you know it's a REDIR, tells you which one
if (tkn_sym_type(input_str + i) == REDIR)
{
    tk->type = REDIR;
    tk->redir = tkn_redir_type(input_str + i);
} */

// return symbol(s) of what's causing syntax err.
char	*syntax_err_check(char *trimmed_input)
{
	char	*syntax_err;

	syntax_err = NULL;
	if (*trimmed_input == '|')
		syntax_err = *trimmed_input; // == trimmed_input[0]
	if (tkn_sym_type(trimmed_input + ft_strlen(trimmed_input) - 2) == REDIR)
		syntax_err = "newline";
	if (syntax_err)
		syntax_err_msg(syntax_err);
	return (syntax_err);
	// while (trimmed_input[i])
	// {
		// while (trimmed_input[i] && ft_isspace(trimmed_input[i]))
		// 	i++;
		// if (!trimmed_input[i])
		// 	break ;
		// while (trimmed_input[i] && !(ft_isspace(trimmed_input[i])))
		// {
		// 	tkn_type_check(trimmed_input[i]);
			// quote_type = quote_check(input_str + i);
			// if (quote_type)
			// 	init_2prompt();
	// 		i++;
	// 	}
	// }
	// return (NULL);
}
