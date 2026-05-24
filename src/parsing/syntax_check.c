/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 17:45:10 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/17 17:58:26 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

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

static int	skip_quote(char *input_str, int i)
{
	char	qmark;

	if (input_str[i] == '\'' || input_str[i] == '"')
	{
		qmark = input_str[i];
		i++;
		while (input_str[i] && input_str[i] != qmark)
			i++;
		if (input_str[i] == qmark)
			i++;
	}
	return (i);
}

static char	*syntax_err_pipe_redir(char *input_str)
{
	int				i;
	t_syntax_err	syntax_check;

	i = 0;
	while (input_str && input_str[i])
	{
		if (input_str[i] == '\'' || input_str[i] == '"')
		{
			i = skip_quote(input_str, i);
			continue ;
		}
		else if (char_is_op(input_str[i]) == true)
		{
			if (input_str[i] == '>' || input_str[i] == '<')
				syntax_check = syntax_err_redir(input_str, i);
			else
				syntax_check = syntax_err_pipe(input_str, i);
			if (syntax_check.err_str)
				return (syntax_check.err_str);
			i = syntax_check.i;
			continue ;
		}
		i++;
	}
	return (NULL);
}

char	*syntax_check(char *input_str)
{
	int		i;
	char	open_qmark;

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
