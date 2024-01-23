/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 11:07:55 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/16 11:07:59 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nbrlen(int n)
{
	int	ln;

	ln = 1;
	if (n < 0)
		ln++;
	while (n / 10 != 0)
	{
		ln++;
		n /= 10;
	}
	return (ln);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	long	l;

	str = malloc((ft_nbrlen(n) + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	i = ft_nbrlen(n);
	str[i] = 0;
	l = n;
	if (l < 0)
	{
		l *= -1;
		str[0] = '-';
	}
	while (i > 0)
	{
		str[i - 1] = (l % 10) + '0';
		l /= 10;
		if (i == 2 && n < 0)
			i = 0;
		i--;
	}
	return (str);
}
