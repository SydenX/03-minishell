/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 11:08:20 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/16 11:08:24 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	l;

	l = (long) n;
	if (l < 0)
	{
		l *= -1;
		ft_putchar_fd('-', fd);
	}
	if (l >= 10)
	{
		ft_putnbr_fd(l / 10, fd);
		l %= 10;
	}
	ft_putchar_fd(l + '0', fd);
}
