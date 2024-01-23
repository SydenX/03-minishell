/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 16:07:49 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/23 13:48:14 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **list, t_list *new)
{
	t_list	*cpy;

	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	cpy = *list;
	while (cpy->next)
	{
		cpy = cpy->next;
	}
	cpy->next = new;
}
