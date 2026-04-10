/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:04:54 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/25 16:31:18 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **list, t_list *new)
{
	if (!list || !new)
		return ;
	new->next = *list;
	*list = new;
}
/*
int	main(void)
{
	int	i = 0;
	char	*str1 = "meow?";
	char	*str2 = "whav or";
	char	*str3 = "Dog says";

	t_list	*node1 = ft_lstnew(str1);
	t_list	*node2 = ft_lstnew(str2);
	t_list	*node3 = ft_lstnew(str3);

	t_list	*list = NULL;

	if (!node1 || !node2 || !node3)
    {
        printf("FAILED !\n");
        return (1);
    }

	ft_lstadd_front(&list, node1);
	ft_lstadd_front(&list, node2);
	ft_lstadd_front(&list, node3);

	t_list	*temp = list;
	while (temp)
	{
		printf("Node content[%d]: %s\n", i, (char *)temp->content);
		temp = temp->next;
		i++;
	}
}*/