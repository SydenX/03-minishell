/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:58:03 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/01 11:44:43 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(char *arg)
{
	if (arg == NULL)
		chdir(getenv("HOME"));
	else if (chdir(arg) != 0)
		printf("cd: no such file or directory: %s\n", arg);
}
