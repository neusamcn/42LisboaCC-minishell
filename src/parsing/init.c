/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 20:24:29 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/18 16:35:31 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"
#include "execution.h"

static char	*input_strs_join(char *input_str, char *extra_input)
{
	char	*tmp;
	char	*full_input;

	tmp = ft_strjoin(input_str, " ");
	if (!tmp)
		return (NULL);
	full_input = ft_strjoin(tmp, extra_input);
	free(tmp);
	return (full_input);
}

static char	*validate_complete_input(char *input_str, t_shelly *shelly)
{
	char	*syntax_err;
	char	*extra_input;
	char	*full_input;

	syntax_err = syntax_check(input_str);
	while (syntax_err && (!(ft_strcmp(syntax_err, "incomplete"))
			|| !(ft_strcmp(syntax_err, "'")) || !(ft_strcmp(syntax_err, "\""))))
	{
		extra_input = put_extra_prompt(shelly, input_str);
		full_input = input_strs_join(input_str, extra_input);
		free(input_str);
		free(extra_input);
		if (!full_input)
			return (NULL);
		input_str = full_input;
		syntax_err = syntax_check(input_str);
	}
	if (syntax_err)
	{
		syntax_err_msg(syntax_err);
		add_history(input_str);
		free(input_str);
		return (NULL);
	}
	return (input_str);
}

static void	read_eval_print_loop(t_shelly *shelly)
{
	char	*input_str;
	t_token	*tokens;
	// char	**tokens; // ft_split(mini_av, ' ' or ft_isspace())

	set_signals_interactive_parent();
	while (1)
	{
		input_str = put_prompt(shelly, "shelly");
		if (!input_str)
			exit_cleanup(EXIT_SUCCESS, shelly);
		if (*input_str)
		{
			input_str = validate_complete_input(input_str, shelly);
			if (input_str)
			{
				add_history(input_str);
				tokens = tokenize_input(input_str);
				exec_loop(tokens, shelly);
				free(input_str);
				// TODO: tokenize + parse + execute here
			}
		}
		free(input_str);
	}
}

static void	non_interactive_mode(t_shelly *shelly)
{
	t_token	*tokens;
	char	*line;

	set_signals_noninteractive();
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		tokens = tokenize_input(line);
		exec_loop(tokens, shelly);
		// TODO: tokenize + expand + execute here
		(void)shelly;
		free(line);
	}
}

t_shelly	*init(char **envp)
{
	t_shelly	*shelly;

	shelly = set_shellyenvp(envp);
	// TODO: initialise other elements of *shelly
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		read_eval_print_loop(shelly);
	else
		non_interactive_mode(shelly);
		// TODO: does this mode not need shelly_envp?
		// Read line from stdin
		// Parse + execute
		// Repeat until EOF
		// Exit with proper status
	return (shelly);
}
