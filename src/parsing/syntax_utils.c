/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 21:21:12 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/11 16:58:09 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

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
	return (0);
}
