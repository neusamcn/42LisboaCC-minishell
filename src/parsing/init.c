/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 20:24:29 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/05 21:57:12 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static void	read_eval_print_loop(t_minishell *minishell)
{
	char	*input_str;
	// char	**tokens; // ft_split(mini_av, ' ' or ft_isspace())

	set_signals_interactive_parent();
	while (1)
	{
		input_str = put_prompt(minishell, "meg&neu-minishell");
		if (!input_str)
			exit_cleanup(EXIT_SUCCESS, minishell);
		if (*input_str)
		{
			add_history(input_str);
			// tokenize_input(input_str);
			// TODO: tokenize + parse + execute here
		}
		free(input_str);
	}
}

static void	non_interactive_mode(t_minishell *minishell)
{
	char	*line;

	set_signals_noninteractive();
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		// TODO: tokenize + parse + execute here
		(void)minishell;
		free(line);
	}
}

t_minishell	*init(char **envp)
{
	t_minishell	*minishell;

	minishell = set_minienvp(envp);
	// TODO: initialise other elements of *minishell
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		read_eval_print_loop(minishell);
	else
		non_interactive_mode(minishell);
		// TODO: does this mode not need minienvp?
	return (minishell);
}
