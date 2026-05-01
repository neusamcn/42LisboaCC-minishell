/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wrappers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 23:44:25 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/01 13:28:07 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

char	*getcwd_protec(char *buf, size_t size, t_minishell *shelly)
{
	if (!getcwd(buf, size))
	{
		// TODO: do I actually need this print? does it have an errno?
		print_err_msg("getcwd failed");
		exit_cleanup(EXIT_FAILURE, shelly);
	}
	return (buf);
}

void	*malloc_protec(size_t size, t_minishell *shelly)
{
	void	*ptr;

	// TODO: do I actually need this first if()? does it have an errno?
	if (size == 0)
	{
		print_err_msg("malloc() failed; size == 0");
		exit_cleanup(EXIT_FAILURE, shelly);
	}
	ptr = malloc(size);
	if (!ptr)
	{
		print_err_msg("malloc() failed");
		exit_cleanup(EXIT_FAILURE, shelly);
	}
	// TODO: add ptr to hashmap
	return (ptr);
}

// TODO: add *shelly to args?
void	*ft_calloc_protec(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(nmemb, size);
	if (!ptr)
	{
		print_err_msg("ft_calloc() failed"); // TODO: test and verify errno
		exit_cleanup(EXIT_FAILURE, NULL);
	}
	// TODO: add ptr to hashmap
	return (ptr);
}
