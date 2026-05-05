/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:47:13 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/03 20:26:04 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/minishell.h"
#include "../include/parsing.h"

void	exit_cleanup(int exit_status, t_shelly *shelly)
{
	int	i;

	ft_putendl_fd("Exiting shelly...", STDOUT_FILENO);
	rl_clear_history();
	i = 0;
	while (shelly->envp[i])
		free(shelly->envp[i++]);
	free(shelly->envp);
	exit(exit_status);
}

int	main(int ac, char **av, char **envp)
{
	t_shelly	*shelly;

	if (ac != 1 || av[1])
	{
		// TODO: print_err_args_msg() ?
		ft_putendl_fd("Invalid arguments to run Minishell. Try "
			ORCHID"./minishell"COLOR_RESET" at "
			ORCHID"dir /minishell"COLOR_RESET" root.", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
	shelly = init(envp);
	// TODO: add **av/*av/av[0] as arg?
	exit_cleanup(EXIT_SUCCESS, shelly);
}
