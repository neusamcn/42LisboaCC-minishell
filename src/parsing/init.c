/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 20:24:29 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/03 23:46:07 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*build_pretty_prompt(char *prompt_str, char *cwd_str)
{
	char	*tmp_prompt[7];
	char	*full_prompt;
	size_t	len;
	int		i;

	tmp_prompt[0] = "\001"DEEP_PINK"\002";
	tmp_prompt[1] = prompt_str;
	tmp_prompt[2] = ":\001"COLOR_RESET"\002";
	tmp_prompt[3] = cwd_str;
	tmp_prompt[4] = "\001"DEEP_PINK"\002$ ";
	tmp_prompt[5] = "\001"COLOR_RESET"\002";
	tmp_prompt[6] = NULL;
	len = 0;
	i = 0;
	while (tmp_prompt[i])
		len += ft_strlen(tmp_prompt[i++]);
	full_prompt = ft_calloc(len + 1, sizeof(char));
	if (!full_prompt)
		exit_cleanup(EXIT_FAILURE);
	i = 0;
	while (tmp_prompt[i])
		ft_strlcat(full_prompt, tmp_prompt[i++], len + 1);
	return (full_prompt);
}

char	*put_prompt(char *prompt_str)
{
	char	cwd_str[PATH_MAX];
	char	*full_prompt;
	char	*input;

	getcwd_protec(cwd_str, PATH_MAX);
	full_prompt = build_pretty_prompt(prompt_str, cwd_str);
	input = readline(full_prompt);
	free(full_prompt);
	return (input);
}

void	read_eval_print_loop(void)
{
	char	*input_str;
	// char	**tokens; // ft_split(mini_av, ' ' or ft_isspace())

	set_signals_interactive_parent();
	while (1)
	{
		input_str = put_prompt("meg&neu minishell");
		if (!input_str)
			exit_cleanup(EXIT_SUCCESS);
		/* TODO: signal handler catches SIGINT => new prompt;
								catches SIGQUIT => does nothing */
		if (*input_str)
		{
			add_history(input_str);
			// tokenize_input(input_str);
			// TODO: tokenize + parse + execute here
		}
		free(input_str);
	}
}

void	non_interactive_mode(void)
{
	char	*line;

	set_signals_noninteractive();
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		// TODO: parse & execute here
		free(line);
	}
}

void	init(char **env)
{
	(void)env;

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		read_eval_print_loop();
	else
		non_interactive_mode();
}
