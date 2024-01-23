/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:41:51 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/15 21:41:52 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int				i;
	unsigned char	*copys1;
	unsigned char	*copys2;

	i = 0;
	copys1 = (unsigned char *) s1;
	copys2 = (unsigned char *) s2;
	while (i < (int)n)
	{
		if (copys1[i] != copys2[i])
			return (copys1[i] - copys2[i]);
		i++;
	}
	return (0);
}
