/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wrappers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 23:44:25 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/03 23:49:08 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*getcwd_protec(char *buf, size_t size)
{
	if (!getcwd(buf, size))
	{
		print_err_msg("getcwd failed");
		exit_cleanup(EXIT_FAILURE);
	}
	return (buf);
}

void	*malloc_protec(size_t size)
{
	void	*ptr;

	if (size == 0)
		print_err_msg("malloc() failed; size == 0");
	ptr = malloc(size);
	if (!ptr)
	{
		print_err_msg("malloc() failed");
		exit_cleanup(EXIT_FAILURE);
	}
	return (ptr);
}
