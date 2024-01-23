/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:51:36 by jtollena          #+#    #+#             */
/*   Updated: 2023/12/01 11:18:43 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_formats(char tmptype, int pln, va_list args, char flag)
{
	if (pln == -1)
		return (-1);
	if (tmptype == 'c')
		pln = ft_putchar(pln, va_arg(args, int));
	else if (tmptype == 'd' || tmptype == 'i')
		pln = ft_putnbr(pln, va_arg(args, int), flag);
	else if (tmptype == 'u')
		pln = ft_putunbr(pln, va_arg(args, unsigned int));
	else if (tmptype == 's')
		pln = ft_putstr(pln, va_arg(args, char *));
	else if (tmptype == 'x')
		pln = ft_putnbr_base(pln, (size_t)va_arg(args, unsigned int), 0, flag);
	else if (tmptype == 'X')
		pln = ft_putnbr_base(pln, (size_t)va_arg(args, unsigned int), 1, flag);
	else if (tmptype == 'p')
	{
		pln = ft_putstr(pln, "0x");
		pln = ft_putnbr_base(pln, va_arg(args, size_t), 0, 0);
	}
	return (pln);
}

int	ft_isflag(char c, char *nexts)
{
	if (c == '+' && (nexts[0] == 'd' || nexts[0] == 'i'))
		return (1);
	if (c == '#' && (nexts[0] == 'x' || nexts[0] == 'X'))
		return (1);
	if (c == ' ' && (nexts[0] == 'd' || nexts[0] == 'i'))
		return (1);
	return (0);
}

int	ft_printf(const char *type, ...)
{
	va_list	args;
	char	*tmptype;
	int		i;
	long	pln;
	char	flag;

	pln = 0;
	tmptype = (char *)type;
	va_start(args, type);
	i = 0;
	while (tmptype[i] != 0)
	{
		while (tmptype[i] != '%' && tmptype[i] != 0)
			pln = ft_putchar(pln, tmptype[i++]);
		if (tmptype[i++] == 0)
			break ;
		if (ft_isflag(tmptype[i], &tmptype[i + 1]) >= 1)
			flag = tmptype[i++];
		if (tmptype[i] == '%')
			pln = ft_putchar(pln, '%');
		pln = ft_formats(tmptype[i++], pln, args, flag);
		flag = 0;
	}
	va_end(args);
	return (pln);
}
