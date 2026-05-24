/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:47:13 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/24 13:00:42 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"
#include "../include/execution.h"

void	exit_cleanup(int exit_status, t_shelly *shelly)
{
	int	fd;

	rl_clear_history();
	fd = 3;
	while (fd < 10)
	{
		if (fcntl(fd, F_GETFD) != -1)
			close(fd);
		fd++;
	}
	if (shelly)
	{
		free_tkn(shelly->cur_tok);
		free_cmd_line(shelly->cur_cmd);
		free_copyenvp(shelly);
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
	//ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
	shelly = init(envp);
	exit_cleanup(EXIT_SUCCESS, shelly);
}
