/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:59:12 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 13:48:11 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_cmd *cmd)
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
	return (0);
}
