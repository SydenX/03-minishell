/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:42:09 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/15 21:42:10 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const unsigned char	*srccpy;
	unsigned char		*copy;

	if (dst == NULL && src == NULL)
		return (NULL);
	copy = dst;
	srccpy = src;
	if (dst > src)
	{
		while (len > 0)
		{
			copy[len - 1] = srccpy[len - 1];
			len--;
		}
	}
	else
		ft_memcpy(dst, src, len);
	return (dst);
}
