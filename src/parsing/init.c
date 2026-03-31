/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 20:24:29 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/01 00:25:34 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init(int ac, char **av, char **env)
{
	char	cwd_str[PATH_MAX];
	char	*input_str;
	// char	**tokens; // ft_split(mini_av, ' ' or ft_isspace())
	(void)ac;
	(void)av;
	(void)env;
	while (1)
	{
		getcwd_protec(cwd_str, PATH_MAX);
		if (isatty(STDIN_FILENO)) // TODO: necessary? review redirections and if more protections are needed
		{
			ft_putstr_fd(DEEP_PINK"meg&neu minishell:"COLOR_RESET, STDOUT_FILENO);
			ft_putstr_fd(cwd_str, STDOUT_FILENO);
			input_str = readline(DEEP_PINK"$ "COLOR_RESET);
			if (!input_str)
				exit_cleanup();
			if (*input_str)
				add_history(input_str);
			// TODO: tokenize + parse + execute here
			free(input_str);
		}
		else
			print_err_msg("not a terminal"); // TODO: should there be a different logic?
	}
}
