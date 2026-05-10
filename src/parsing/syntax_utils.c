/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 21:21:12 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/26 18:02:26 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	ft_isspace(int c)
{
	// TODO: consider less space types?
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	return (0);
}

char	quote_check(char *str)
{
	int	i;

	i = 0;
	if (str[i] == 39 || str[i] == 34)
	{
		while (str[++i])
		{
			if (str[i] == str[0])
				return (str[i]);
		}
	}
	return (NULL);
}
