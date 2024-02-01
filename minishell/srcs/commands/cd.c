/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:58:03 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/01 13:54:30 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(char *arg)
{
	if (arg == NULL)
		cd(getenv("HOME"));
	else if (chdir(arg) != 0)
	{
		printf("cd: no such file or directory: %s\n", arg);
		exit_code = 1;
	}
	else
		exit_code = 0;
}
