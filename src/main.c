/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:47:13 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/05 16:37:51 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/minishell.h"
#include "../include/parsing.h"

void	exit_cleanup(int exit_status, t_minishell *minishell)
{
	int	i;

	ft_putendl_fd("Exiting minishell...", STDOUT_FILENO);
	rl_clear_history();
	i = 0;
	while (minishell->minienvp[i])
		free(minishell->minienvp[i++]);
	free(minishell->minienvp);
	// TODO: function to close all previously opened fd?
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	// TODO: can i close stderr before sending an exit status?
	exit(exit_status);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	*minishell;

	if (ac != 1 || av[1])
	{
		// TODO: print_err_args_msg() ?
		ft_putendl_fd("Invalid arguments to run Minishell. Try "
			ORCHID"./minishell"COLOR_RESET" at "
			ORCHID"dir /minishell"COLOR_RESET" root.", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
	minishell = init(envp);
	// TODO: add **av/*av/av[0] as arg?
	exit_cleanup(EXIT_SUCCESS, minishell);
}
