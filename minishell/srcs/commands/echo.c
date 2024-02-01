/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 12:11:25 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/01 13:54:09 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(char *out, int has_param)
{
	if (has_param)
	{
		if (ft_strncmp(out, "$?", 2) == 0)
			printf("%d", exit_code);
		else
			printf("%s", out);
	}
	else
	{
		if (ft_strncmp(out, "$?", 2) == 0)
			printf("%d\n", exit_code);
		else
			printf("%s\n", out);
	}
	// exit_code = 0;
}
