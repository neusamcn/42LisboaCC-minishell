/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:44:07 by megiazar          #+#    #+#             */
/*   Updated: 2025/04/27 17:57:25 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_clear(t_list *result, void (*del)(void *), void *tmp)
{
	ft_lstclear(&result, del);
	del(tmp);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*result;
	t_list	*tmp;
	void	*new;

	if (!lst || !f || !del)
		return (NULL);
	result = NULL;
	while (lst != NULL)
	{
		tmp = f(lst->content);
		if (!tmp)
		{
			ft_clear(result, del, tmp);
			return (NULL);
		}
		new = ft_lstnew(tmp);
		if (!new)
		{
			ft_clear(result, del, tmp);
			return (NULL);
		}
		ft_lstadd_back(&result, new);
		lst = lst->next;
	}
	return (result);
}
/* static void *ft_example(void *ptr)
{
	if (!ptr)
		return (NULL);
    char *new_str = ft_strdup((char *)ptr);
    if (!new_str)
        return (NULL);
    printf("%s\n", "AIII PAPI");
    return (new_str);
} */

/* int main(void)
{
    t_list *node = ft_lstnew(ft_strdup("hello"));
	t_list *node2 = ft_lstnew(ft_strdup("m"));
    t_list *node3 = ft_lstnew(ft_strdup("bye"));

    node->next = node2;
    node2->next = node3;
	node3->next = NULL;

    t_list *new = ft_lstmap(node, ft_example, free);

    t_list *tmp = new;
    while (tmp)
    {
        printf("%s\n", (char *)tmp->content);
        tmp = tmp->next;
    }
    ft_lstclear(&node, free);
    ft_lstclear(&new, free);
    return (0);
} */

/* int main(void)
{
 t_list *list = NULL;
 t_list *node1 = malloc(sizeof(t_list));
 t_list *node2 = malloc(sizeof(t_list));
 t_list *node3 = malloc(sizeof(t_list));

 node1->content = malloc(sizeof(int));
 node2->content = malloc(sizeof(int));
 node3->content = malloc(sizeof(int));

 list = node1;

 *(int *)node1->content = 10;
 node1->next = node2;

 *(int *)node2->content = 20;
 node2->next = node3;

 *(int *)node3->content = 30;
 node3->next = NULL;

 t_list *temp = list;
 while (temp != NULL)
 {
  printf("%i ->", *(int *)temp->content);
  temp = temp->next;
 }
 printf("\n");
 t_list *newlist = ft_lstmap(list, f, ft_clear);
 temp = newlist;
 while (temp != NULL)
 {
  printf("%i ->", *(int *)temp->content);
  temp = temp->next;
 }
 return (0);
} 
 static void	*f(void *lst)
{
	*(int *)lst += 10;
	return (lst);
}*/