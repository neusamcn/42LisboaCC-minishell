/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wrappers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 23:44:25 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/22 19:44:52 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

char	*getcwd_protec(char *buf, size_t size, t_shelly *shelly)
{
	if (!getcwd(buf, size))
	{
		print_err_msg("getcwd failed");
		exit_cleanup(EXIT_FAILURE, shelly);
	}
	return (buf);
}

void	*malloc_protec(size_t size, t_shelly *shelly)
{
	void	*ptr;

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
	return (ptr);
}

void	*ft_calloc_protec(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(nmemb, size);
	if (!ptr)
	{
		print_err_msg("ft_calloc() failed");
		exit_cleanup(EXIT_FAILURE, NULL);
	}
	return (ptr);
}
