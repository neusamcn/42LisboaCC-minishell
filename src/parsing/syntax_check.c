/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 17:45:10 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/17 14:07:54 by ncruz-ne         ###   ########.fr       */
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

static t_syntax_err	syntax_err_redir_afterspace(char *input_str, int i)
{
	t_syntax_err	syntax_check;

	syntax_check.err_str = NULL;
	while (input_str[i] && ft_isspace(input_str[i]) == true)
		i++;
	syntax_check.i = i;
	if (input_str[i] == 0)
	{
		syntax_check.err_str = "newline";
		return (syntax_check);
	}
	else if (input_str[i] == '>' || input_str[i] == '<' || input_str[i] == '|')
	{
		if (input_str[i] != 0 && input_str[i] == input_str[i + 1])
		{
			syntax_check.i++;
			if (input_str[i] == '>')
			{
				syntax_check.err_str = ">>";
				return (syntax_check);
			}
			syntax_check.err_str = "<<";
			return (syntax_check);
		}
		if (input_str[i] == '>')
			syntax_check.err_str = ">";
		else if (input_str[i] == '<')
			syntax_check.err_str = "<";
		else if (input_str[i] == '|')
			syntax_check.err_str = "|";
		return (syntax_check);
	}
	return (syntax_check);
}

static t_syntax_err	syntax_err_redir(char *input_str, int i)
{
	t_syntax_err	syntax_check;

	syntax_check.err_str = NULL;
	if (input_str[i] == '>' || input_str[i] == '<')
	{
		i++;
		if (input_str[i] && input_str[i - 1] == '<' && input_str[i] == '>')
		{
			syntax_check.err_str = "newline";
			syntax_check.i = i;
			return (syntax_check);
		}
		else if (input_str[i] && input_str[i] == input_str[i - 1])
			i++;
		syntax_check = syntax_err_redir_afterspace(input_str, i);
		if (syntax_check.err_str)
			return (syntax_check);
	}
	syntax_check.i = i;
	return (syntax_check);
}

static t_syntax_err	syntax_err_pipe(char *input_str, int i)
{
	t_syntax_err	syntax_check;

	syntax_check.err_str = NULL;
	if (input_str[i] == '|')
		i++;
	while (input_str[i] && ft_isspace(input_str[i]) == true)
		i++;
	if (input_str[i] == 0)
		syntax_check.err_str = "incomplete";
	else if (input_str[i] == '|')
		syntax_check.err_str = "|";
	syntax_check.i = i;
	return (syntax_check);
}

static char	*syntax_err_pipe_redir(char *input_str)
{
	int				i;
	t_syntax_err	syntax_check;
	char			qmark;

	i = 0;
	while (input_str && input_str[i])
	{
		if (input_str[i] == '\'' || input_str[i] == '"')
		{
			qmark = input_str[i];
			i++;
			while (input_str[i] && input_str[i] != qmark)
				i++;
			if (input_str[i] == qmark)
				i++;
			continue ;
		}
		else if (input_str[i] && input_str[i] == '|')
		{
			syntax_check = syntax_err_pipe(input_str, i);
			if (syntax_check.err_str)
				return (syntax_check.err_str);
			i = syntax_check.i;
			continue ;
		}
		else if (input_str[i] && (input_str[i] == '>' || input_str[i] == '<'))
		{
			syntax_check = syntax_err_redir(input_str, i);
			if (syntax_check.err_str)
				return (syntax_check.err_str);
			i = syntax_check.i;
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
