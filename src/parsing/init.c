/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 20:24:29 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/01 14:55:28 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Milena, this file is crazy right now, don't freak out :(

// static char	find_unclosed_quote_rec(char *input_str, char open_qmark)
// {
// 	if (!input_str || *input_str == '\0')
// 		return (open_qmark);
// 	if (open_qmark == '\0' && (*input_str == '\'' || *input_str == '"'))
// 		return (find_unclosed_quote_rec(input_str + 1, *input_str));
// 	if (open_qmark != '\0' && *input_str == open_qmark)
// 		return (find_unclosed_quote_rec(input_str + 1, '\0'));
// 	return (find_unclosed_quote_rec(input_str + 1, open_qmark));
// }

// static char	quote_err_check(char *input_str)
// {
// 	char	unclosed;

// 	unclosed = find_unclosed_quote_rec(input_str, '\0');
// 	if (unclosed == '\'' || unclosed == '"')
// 		return (unclosed);
// 	return (0);
// }

// static char	*trim_check(char *input_str)
// {
// 	char	*trimmed_input;

// 	// 2nd prompt isn't needed:
// 	trimmed_input = ft_strtrim(input_str, "\t\n\v\f\r "); // consider less space types?
// 	if (!trimmed_input)
// 		return (input_str);
// 	if (*trimmed_input == '\0' || syntax_err_check(trimmed_input))
// 	{
// 		free(trimmed_input);	
// 		return (input_str);
// 	}
// 	return (NULL);
// }

// TODO: add to prompt.c ?
// return full input with all input strs appended
// static char	*put_2ary_prompt(char *input_str, t_shelly *shelly)
// {
// 	char	*trimmed_input;
// 	char	*extra_prompt;
// 	char	*input_str_nl;
// 	char	*full_prompt;
// 	// char	open_qmark;

// 	// 2nd prompt isn't needed:
// 	trimmed_input = ft_strtrim(input_str, "\t\n\v\f\r "); // consider less space types?
// 	if (!trimmed_input)
// 		return (input_str);
// 	if (*trimmed_input == '\0' || syntax_err_check(trimmed_input))
// 	{
// 		free(trimmed_input);
// 		return (input_str);
// 	}
// 	full_prompt = input_str;
// 	// trigger 2nd prompt and check for syntax errs again after building new input_str
// 	while (quote_err_check(trimmed_input) || ft_strnstr(trimmed_input, "<<", 2)
// 		|| *(trimmed_input + ft_strlen(trimmed_input) - 1) == '|')
// 	{
// 		// prompt (readline + add memory chars to input_str)
// 		free(trimmed_input);
// 		extra_prompt = readline("> ");
// 		if (!extra_prompt)
// 			exit_cleanup(EXIT_SUCCESS, shelly);
// 		input_str_nl = ft_strjoin(full_prompt, "\n");
// 		free(full_prompt);
// 		full_prompt = ft_strjoin(input_str_nl, extra_prompt);
// 		if (!full_prompt)
// 			return (free(input_str_nl), free(extra_prompt));
// 		free(input_str_nl);
// 		free(extra_prompt);
// 		// check if another 2ndary prompt isn't needed
// 		trimmed_input = ft_strtrim(full_prompt, "\t\n\v\f\r "); // consider less space types?
// 		if (!trimmed_input)
// 			break ;
// 		if (*trimmed_input == '\0' || syntax_err_check(trimmed_input))
// 		{
// 			free(trimmed_input);
// 			break ;
// 		}
// 	}
// 	return (full_prompt);
// }

static void	read_eval_print_loop(t_shelly *shelly)
{
	char	*input_str;
	// char	**tokens; // ft_split(mini_av, ' ' or ft_isspace())

	set_signals_interactive_parent();
	while (1)
	{
		input_str = put_prompt(shelly, "shelly");
		if (!input_str)
			exit_cleanup(EXIT_SUCCESS, shelly);
		if (*input_str)
		{
			// input_str = put_2ary_prompt(input_str, shelly);
			// add_history(input_str);
			// tokenize_input(input_str);
			// TODO: tokenize + parse + execute here
		}
		free(input_str);
	}
}

static void	non_interactive_mode(t_shelly *shelly)
{
	char	*line;

	set_signals_noninteractive();
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		// TODO: tokenize + parse + execute here
		(void)shelly;
		free(line);
	}
}

t_shelly	*init(char **envp)
{
	t_shelly	*shelly;

	shelly = set_minienvp(envp);
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
