/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:47:13 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/23 22:41:07 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

void	exit_cleanup(int exit_status, t_shelly *shelly)
{
	int	i;
	int fd;

	//ft_putendl_fd("exit", STDOUT_FILENO);
	rl_clear_history();
	fd = 3;
	while (fd < 10)
		close(fd++);
	if (shelly)
	{
		i = 0;
		while (shelly->envp[i])
			free(shelly->envp[i++]);
		free(shelly->envp);
		free(shelly);
	}
	exit(exit_status);
}

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
	ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
	shelly = init(envp);
	exit_cleanup(EXIT_SUCCESS, shelly);
}
