/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:47:13 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/03 20:51:43 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_cleanup(int exit_status)
{
	ft_putendl_fd("Exiting minishell...", STDOUT_FILENO);
	rl_clear_history();
	exit(exit_status);
}

int	main(int ac, char **av, char **env)
{
	if (ac != 1 || av[1])
	{
		ft_putendl_fd("Minishell couldn't start. Try "
			ORCHID"./minishell"COLOR_RESET, STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
	init(env);
	exit_cleanup(EXIT_SUCCESS);
}
