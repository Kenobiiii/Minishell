/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:34:49 by paromero          #+#    #+#             */
/*   Updated: 2023/10/31 12:22:26 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*list2;
	t_list	*tmp;
	t_list	*data;

	list2 = NULL;
	while (lst)
	{
		data = f(lst->content);
		tmp = ft_lstnew(data);
		if (!tmp)
		{
			free (data);
			ft_lstclear(&list2, del);
			return (NULL);
		}
		ft_lstadd_back(&list2, tmp);
		lst = lst->next;
	}
	return (list2);
}
