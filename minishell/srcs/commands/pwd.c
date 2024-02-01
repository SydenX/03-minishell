/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:59:12 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/01 13:48:22 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd()
{
	char	cwd[1025];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		exit_code = 0;
		printf("%s\n", cwd);
	}
	else
		exit_code = 1;
}
