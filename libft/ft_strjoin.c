/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:20:34 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/29 16:39:51 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t		index;
	size_t		size;
	char		*str;
	const char	*tmp1;
	const char	*tmp2;

	tmp1 = s1;
	tmp2 = s2;
	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc((size + 1) * sizeof(char));
	if (!str)
		return (NULL);
	index = 0;
	while (*tmp1)
		str[index++] = *tmp1++;
	while (*tmp2)
		str[index++] = *tmp2++;
	str[index] = '\0';
	return (str);
}
/*
int	main(void)
{
	char	*str_preffix = "dogs says ";
	char	*str_suffix = "meow meow";

	printf("The result is: %s\n", ft_strjoin(str_preffix, str_suffix));
	return (0);
}*/