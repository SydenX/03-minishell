/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:59:12 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/05 10:51:01 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd()
{
	char	*cwd;

	cwd = NULL;
	if ((cwd = getcwd(cwd, 0)) != NULL)
	{
		exit_code = 0;
		printf("%s\n", cwd);
	}
	else
		exit_code = 1;
}
