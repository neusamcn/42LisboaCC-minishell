/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:55:18 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/26 17:12:19 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*save;

	if (!lst || !*lst || !del)
		return ;
	while (*lst)
	{
		save = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = save;
	}
	*lst = NULL;
}
/*
int	main(void)
{
	char	*str1 = ft_strdup("Drink me");
	char	*str2 = ft_strdup("Keep me");
	char	*str3 = ft_strdup("Delete me");
	char	*str4 = ft_strdup("Forget me");

	t_list	*node1 = ft_lstnew(str1);
	t_list	*node2 = ft_lstnew(str2);
	t_list	*node3 = ft_lstnew(str3);
	t_list	*node4 = ft_lstnew(str4);

	node1->next = node2;
	node2->next = node3;
	node3->next = node4;

	printf("The strings\n%s\n%s\n%s\n%s\n", str1, str2, str3, str4);
	
	ft_lstclear(&node3, free);
	node1->next = node2;
	node2->next = NULL;

	t_list	*example = node1;
	while (example)
	{
		printf("After cleaning: %s\n", (char *)example->content);
		example = example->next;
	}
	return (0);
}*/