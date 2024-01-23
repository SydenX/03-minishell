/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 16:08:53 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/23 12:49:10 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		ln;

	if (!lst)
		return (0);
	ln = 0;
	while (lst)
	{
		lst = lst->next;
		ln++;
	}
	return (ln);
}
