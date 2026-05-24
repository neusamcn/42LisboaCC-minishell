/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_pipe_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:58:45 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/17 18:00:02 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

bool	char_is_op(char c)
{
	if (c && (c == '>' || c == '<' || c == '|'))
		return (true);
	return (false);
}

static t_syntax_err	ops_afterspace(char *input_str, int i)
{
	t_syntax_err	syntax_check;

	syntax_check.err_str = NULL;
	if (char_is_op(input_str[i]) == true)
	{
		syntax_check.i = i;
		if (input_str[i] != 0 && input_str[i] == input_str[i + 1])
		{
			syntax_check.i++;
			if (input_str[i] == '>')
				syntax_check.err_str = ">>";
			else if (input_str[i] == '<')
				syntax_check.err_str = "<<";
			if (syntax_check.err_str)
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
	else if (char_is_op(input_str[i]) == true)
	{
		syntax_check = ops_afterspace(input_str, i);
		if (syntax_check.err_str)
			return (syntax_check);
		i = syntax_check.i;
	}
	return (syntax_check);
}

t_syntax_err	syntax_err_redir(char *input_str, int i)
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

t_syntax_err	syntax_err_pipe(char *input_str, int i)
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
