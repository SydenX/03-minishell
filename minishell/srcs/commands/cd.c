/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:58:03 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 15:03:20 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(t_cmd *cmd, char **envp)
{
	char	*home;

	if (cmd->args == NULL)
	{
		home = getenv("HOME");
		if (home == NULL)
		{
			printf("cd: home path is not set\n");
			exit(1);
		}
		if (chdir(home) != 0)
		{
			printf("cd: no such file or directory: %s\n", cmd->args[0]);
			exit(1);	
		}
		exit (0);
	}
	else if (chdir(cmd->args[0]) != 0)
	{
		printf("cd: no such file or directory: %s\n", cmd->args[0]);
		exit(1);
	}
	else
		exit(0);
}
