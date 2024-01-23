/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:42:57 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/15 21:43:00 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_find(const char *src, const char *tofind, size_t max)
{
	int	i;

	i = 0;
	while ((size_t)i < max && tofind[i] == src[i] && tofind[i] != 0)
	{
		if ((int)ft_strlen(tofind) - 1 == i)
			return (1);
		if (tofind[i] != 0 && tofind[i] != src[i])
			return (0);
		i++;
	}
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	int	i;

	i = 0;
	if (ft_strlen(needle) == 0)
		return ((char *) haystack);
	while ((size_t)i < n && haystack[i] != 0)
	{
		if (haystack[i] == needle[0])
		{
			if (ft_find(&haystack[i], needle, n - (size_t)i) == 1)
				return ((char *) &haystack[i]);
		}
		i++;
	}
	return (NULL);
}
