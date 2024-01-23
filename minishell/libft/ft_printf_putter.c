/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_putter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:40:42 by jtollena          #+#    #+#             */
/*   Updated: 2023/12/01 11:18:13 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(int current, char c)
{
	int	ln;

	if (current == -1)
		return (-1);
	ln = write(1, &c, 1);
	if (ln == -1)
		return (-1);
	else
		return (current + ln);
}

int	ft_putstr(int current, char *s)
{
	int	ln;

	ln = current;
	if (current == -1)
		return (-1);
	if (!s)
		return (ft_putstr(current, "(null)"));
	while (*s != 0)
	{
		ln = ft_putchar(ln, *s);
		s++;
	}
	return (ln);
}

int	ft_putnbr_base(int current, size_t nbr, int maj, char flag)
{
	char	*base;
	int		len;

	if (current == -1)
		return (-1);
	len = current;
	if (maj == 0)
		base = ft_strdup("0123456789abcdef");
	else 
		base = ft_strdup("0123456789ABCDEF");
	if (base == NULL)
		return (-1);
	if (nbr < 0)
	{
		nbr *= -1;
		len = ft_putchar(len, '-');
	}
	else if (flag == '#')
		len = ft_putstr(len, "0x");
	if (nbr / 16 > 0)
		len = ft_putnbr_base(len, nbr / 16, maj, 0);
	len = ft_putchar(len, base[nbr % 16]);
	free(base);
	return (len);
}

int	ft_putnbr(int current, int n, char flag)
{
	long	l;
	int		len;

	if (current == -1)
		return (-1);
	len = current;
	l = (long) n;
	if (l < 0)
	{
		l *= -1;
		len = ft_putchar(len, '-');
	}
	else if (flag == '+' || flag == ' ')
		len = ft_putchar(len, flag);
	if (l >= 10)
	{
		len = ft_putnbr(len, l / 10, 0);
		l %= 10;
	}
	len = ft_putchar(len, l + '0');
	return (len);
}

int	ft_putunbr(int current, unsigned int n)
{
	unsigned long	l;
	int				len;

	if (current == -1)
		return (-1);
	len = current;
	l = (unsigned long) n;
	if (l < 0)
	{
		l *= -1;
		len = ft_putchar(len, '-');
	}
	if (l >= 10)
	{
		len = ft_putunbr(len, l / 10);
		l %= 10;
	}
	len = ft_putchar(len, l + '0');
	return (len);
}
