/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:59:05 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/25 16:31:47 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	int		i;
	int		size;

	i = 0;
	if (!lst)
		return (NULL);
	size = ft_lstsize(lst);
	while (i < size - 1)
	{
		lst = lst->next;
		i++;
	}
	return (lst);
}
/*
int	main(void)
{
	char	*str = "First child";
	char	*str2 = "Middle child";
	char	*str3 = "Youngest child";

	t_list	*node1 = ft_lstnew(str);
	t_list	*node2 = ft_lstnew(str2);
	t_list	*node3 = ft_lstnew(str3);

	node1->next = node2;
	node2->next = node3;
	node3->next = NULL;

	t_list	*last = ft_lstlast(node1);
	printf("The last string iiiiiis %s\n", (char *)last->content);
	free(node1);
	free(node2);
	free(node3);
	return (0);
}*/