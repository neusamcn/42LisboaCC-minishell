/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:27:04 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/25 16:33:45 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
/*
int main(void)
{
    t_list *node1 = ft_lstnew("meow");
    t_list *node2 = ft_lstnew("whav or");
    t_list *node3 = ft_lstnew("Dog says");

   // ft_lstadd_front(&node1, node2);
   // ft_lstadd_front(&node1, node3);

	node1->next = node2;
	node2->next = node3;
	node3->next = NULL;

    printf("List size: %d\n", ft_lstsize(&node1));
    free(node1);
	free(node2);
	free(node3);
    return 0;
}*/