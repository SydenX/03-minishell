/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:59:12 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 14:56:46 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_cmd *cmd, char **envp)
{
	char	*cwd;

	cwd = NULL;
	if ((cwd = getcwd(cwd, 0)) != NULL)
	{
		printf("%s\n", cwd);
		exit(0);
	}
	else
		exit(1);
}
