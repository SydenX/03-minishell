/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:19:14 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/05 10:40:28 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	replace(char **str, char to_replace, char replace_by)
{
	int	i;
	int	ln;

	ln = 0;
	i = 0;
	while (str[0][i] != 0)
	{
		if (str[0][i] == to_replace)
		{
			if (replace_by == 0)
				ln++;
			str[0][i] = replace_by;
		}
		i++;
	}
	if (ln > 0)
		return (ln);
	return (ft_strlen(*str));
}