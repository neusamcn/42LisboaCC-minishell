/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 17:45:10 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/11 00:48:28 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

bool	ft_isspace(int c)
{
	// TODO: consider less space types?
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (true);
	return (false);
}

static char	quote_check(char *input_str)
{
	char	open_qmark;

	open_qmark = 0;
	while (input_str && *input_str)
	{
		if (!open_qmark && (*input_str == '\'' || *input_str == '"'))
			open_qmark = *input_str;
		else if (open_qmark && *input_str == open_qmark)
			open_qmark = 0;
		input_str++;
	}
	return (open_qmark);
}

static char	*syntax_err_redir(char *input_str)
{
	int	i;

	i = 0;
	if (input_str[i] == '>' || input_str[i] == '<')
	{
		i++;
		if (input_str[i] && input_str[i] == input_str[i - 1])
			i++;
		while (input_str[i] && ft_isspace(input_str[i]) == true)
			i++;
		if (input_str[i] == 0)
			return ("incomplete");
		if (input_str[i] == '|' || input_str[i] == '>' || input_str[i] == '<')
			return ("newline");
	}
	return (NULL);
}

static char	*syntax_err_pipe_redir(char *input_str)
{
	int		i;
	char	*syntax_err_redir_str;

	i = 0;
	while (input_str && input_str[i])
	{
		if (input_str[i] == '|')
		{
			i++;
			while (input_str[i] && ft_isspace(input_str[i]) == true)
				i++;
			if (input_str[i] == 0)
				return ("incomplete");
			if (input_str[i] == '|')
				return ("|");
			continue ;
		}
		else if (input_str[i] == '>' || input_str[i] == '<')
		{
			syntax_err_redir_str = syntax_err_redir(input_str + i);
			if (syntax_err_redir_str)
				return (syntax_err_redir_str);
			i++;
			if (input_str[i + 1] && input_str[i + 1] == input_str[i])
				i++;
			continue ;
		}
		else
			i++;
	}
	return (NULL);
}

char	*syntax_check(char *input_str)
{
	int		i;
	char	open_qmark;

	// errs at begining of input_str:
	if (!input_str)
		return ("newline");
	i = 0;
	while (input_str[i] && ft_isspace(input_str[i]) == true)
		i++;
	if (input_str[i] == 0)
		return ("newline");
	if (input_str[i] == '|')
		return ("|");
	open_qmark = quote_check(input_str);
	if (open_qmark == '\'' || open_qmark == '"')
	{
		if (open_qmark == '\'')
			return ("'");
		return ("\"");
	}
	return (syntax_err_pipe_redir(input_str));
}

// TODO: after full testing above OK, delete all below
// static char	*trim_check(char *input_str)
// {
// 	char	*trimmed_input;

// 	// 2nd prompt isn't needed:
// 	trimmed_input = ft_strtrim(input_str, "\t\n\v\f\r "); // consider less space types?
// 	if (!trimmed_input)
// 		return (input_str);
// 	if (*trimmed_input == '\0' || syntax_check(trimmed_input))
// 	{
// 		free(trimmed_input);	
// 		return (input_str);
// 	}
// 	return (NULL);
// }

// return symbol(s) of what's causing syntax err.
// char	*syntax_check(char *input_str)
// {
// 	char	*syntax_err;
// 	char	*trimmed_input;

// 	syntax_err = NULL;
// 	trimmed_input = trim_check(input_str);
// 	if (*trimmed_input == '|')
// 		syntax_err = *trimmed_input; // == trimmed_input[0]
// 	if (tkn_sym_type(trimmed_input + ft_strlen(trimmed_input) - 2) == REDIR)
// 		syntax_err = "newline";
// 	if (syntax_err)
// 		syntax_err_msg(syntax_err);
// 	return (syntax_err);
// 	// while (trimmed_input[i])
// 	// {
// 		// while (trimmed_input[i] && ft_isspace(trimmed_input[i]))
// 		// 	i++;
// 		// if (!trimmed_input[i])
// 		// 	break ;
// 		// while (trimmed_input[i] && !(ft_isspace(trimmed_input[i])))
// 		// {
// 		// 	tkn_type_check(trimmed_input[i]);
// 			// quote_type = quote_check(input_str + i);
// 			// if (quote_type)
// 			// 	init_2prompt();
// 	// 		i++;
// 	// 	}
// 	// }
// 	// return (NULL);
// }
