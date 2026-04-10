/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:09:49 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/25 20:11:30 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}
/*
int	main(void)
{
	int	i = 0;
	char	*str1 = ft_strdup("Dog says");
	char	*str2 = ft_strdup("whuav whuav");
	char	*str3 = ft_strdup("meow");

	t_list	*node1 = ft_lstnew(str1);
	t_list	*node2 = ft_lstnew(str2);
	t_list	*node3 = ft_lstnew(str3);

	node1->next = node2;
	node2->next = node3;

	t_list	*list = NULL;

	ft_lstdelone(node2, free);
	node1->next = node3;

	t_list	*temp = node1;
	while (temp)
	{
		printf("Alive string[%d]: %s\n", i, (char *)temp->content);
		temp = temp->next;
		i++;
	}
	ft_lstdelone(node1, free);
	ft_lstdelone(node3, free);
	return (0);
}*/