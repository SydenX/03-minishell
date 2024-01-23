/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:42:36 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/23 13:10:56 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	int	i;
	int	dstln;

	if (!dst && !src)
		return (0);
	dstln = 0;
	while (dst[dstln] != 0 && dstsize > (size_t)dstln)
		dstln++;
	i = 0;
	while ((size_t)(i + dstln + 1) < dstsize && src[i] != 0)
	{
		dst[dstln + i] = src[i];
		i++;
	}
	if (i > 0)
		dst[dstln + i] = 0;
	return (dstln + ft_strlen(src));
}
