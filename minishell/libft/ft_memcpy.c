/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:42:00 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/15 21:42:04 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const unsigned char	*srccpy;
	unsigned char		*copy;

	copy = dst;
	srccpy = src;
	if (dst == NULL && src == NULL)
		return (0);
	while (n > 0)
	{
		*copy = *srccpy;
		srccpy++;
		copy++;
		n--;
	}
	return (dst);
}
