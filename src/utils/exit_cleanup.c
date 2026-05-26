/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:34:49 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/25 23:30:49 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"
#include "../../include/execution.h"

void	add_ptr_shelly(void *ptr, t_shelly *shelly)
{
	int		row;
	void	**new_ptrs;

	if (!ptr || !shelly)
		return ;
	if (!shelly->malloc_ptrs)
	{
		shelly->malloc_ptrs = ft_calloc_protec(2, sizeof(void *));
		shelly->malloc_ptrs[0] = ptr;
		return ;
	}
	row = 0;
	while (shelly->malloc_ptrs[row])
		row++;
	new_ptrs = ft_calloc_protec(row + 2, sizeof(void *));
	row = 0;
	while (shelly->malloc_ptrs[row])
	{
		new_ptrs[row] = shelly->malloc_ptrs[row];
		row++;
	}
	new_ptrs[row] = ptr;
	free(shelly->malloc_ptrs);
	shelly->malloc_ptrs = new_ptrs;
}

static void	free_ptrs(t_shelly *shelly)
{
	int	row;

	if (!shelly || !shelly->malloc_ptrs)
		return ;
	row = 0;
	while (shelly->malloc_ptrs[row])
		free(shelly->malloc_ptrs[row++]);
	free(shelly->malloc_ptrs);
	shelly->malloc_ptrs = NULL;
}

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
		free_ptrs(shelly);
		free_tkn(shelly->cur_tok);
		free_cmd_line(shelly->cur_cmd);
		free_copyenvp(shelly);
		free(shelly);
	}
	exit(exit_status);
}
