/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:12:08 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/27 14:36:57 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
/*
static void	ft_example(void *ptr)
{
	printf("%s\n", "AIII PAPI"); 
}

int	main(void)
{
	t_list	*node = ft_lstnew("hi");
	t_list	*node2 = ft_lstnew("bye");

	node->next = node2;
	node2->next = NULL;

	t_list	*tmp = node;
	while (tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	tmp = node;
	ft_lstiter(tmp, ft_example);
	free(node);
	free(node2);
	return (0);
}*/
