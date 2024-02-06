/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 12:11:25 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 12:00:48 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_cmd *cmd, char **env)
{
	int	i;
	int	j;

	i = 1;
	while (cmd->args[i] != 0)
	{
		ft_strtrim(cmd->args[i], "\"");
		j = 0;
		while (cmd->args[i][j] != 0)
		{
			if (cmd->args[i][j] == '$' && cmd->args[i][j + 1] == '?')
			{
				j += 2;
				printf("%d", exit_code);
			}
			else
				printf("%c", cmd->args[i][j++]);	
		}
		i++;
	}
	if (cmd->flags == NULL)
		printf("\n");
	exit_code = 0;
}
