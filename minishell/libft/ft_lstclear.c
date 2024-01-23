/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 16:08:05 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/23 13:14:39 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*cpy;
	t_list	*lastcpy;

	if (!del || !lst)
		return ;
	if (!*lst)
		return ;
	cpy = *lst;
	while (cpy)
	{
		del(cpy->content);
		lastcpy = cpy->next;
		free(cpy);
		cpy = lastcpy;
	}
	*lst = NULL;
}
