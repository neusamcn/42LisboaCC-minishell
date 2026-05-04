/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:03:33 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/03 16:27:37 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Milena, I created this file because of compilation error
#include "../../include/execution.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	len_s1;
	int	len_s2;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (len_s1 >= len_s2)
		return (ft_strncmp(s1, s2, ft_strlen(s1)));
	return (ft_strncmp(s1, s2, ft_strlen(s2)));
}


