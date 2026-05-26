/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:47:13 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/25 19:41:24 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"
#include "../include/execution.h"

int	main(int ac, char **av, char **envp)
{
	t_shelly	*shelly;

	if (ac != 1 || av[1])
	{
		ft_putendl_fd("Invalid arguments to run Minishell. Try "
			ORCHID"./minishell"COLOR_RESET" at "
			ORCHID"dir /minishell"COLOR_RESET" root.", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	//ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
	shelly = init(envp);
	exit_cleanup(EXIT_SUCCESS, shelly);
}
