/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 20:58:03 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/25 15:11:11 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (node == NULL)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
/*
int	main(void)
{
	char	*str = "meow dog";
	t_list	*node = ft_lstnew(str);

	if (!node)
	{
		printf("FAILED !\n");
		return (1);
	}
	printf("Node content is %s\n", (char *)node->content);
	if (node->next == NULL)
		printf("NULL as expected");
	free(node);
	return (0);
}*/