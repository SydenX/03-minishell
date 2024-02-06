/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 12:11:25 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 13:16:00 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print(char *str, int single_quoted)
{
	int	j;
	j = 0;
	while (str[j] != 0)
	{
		if (str[j] == '$' && str[j + 1] == '?' && !single_quoted)
		{
			j += 2;
			printf("%d", exit_code);
		}
		else
			printf("%c", str[j++]);	
	}
}

int	echo(t_cmd *cmd, char **env)
{
	int		i;
	char	*newarg;

	i = 1;
	while (cmd->args[i] != 0)
	{
		if (cmd->args[i][0] == '"' && cmd->args[i][ft_strlen(cmd->args[i]) - 1] == '"')
		{
			newarg = ft_strtrim(cmd->args[i], "\"");
			if (newarg == NULL)
				exit(1);
			if (newarg[0] == '\'' && newarg[ft_strlen(newarg) - 1] == '\'')
				print(newarg, 1);
			else
				print(newarg, 0);
		}
		else if (cmd->args[i][0] == '\'' && cmd->args[i][ft_strlen(cmd->args[i]) - 1] == '\'')
			print(cmd->args[i], 1);
		else
			print(cmd->args[i], 0);
		i++;
	}
	if (cmd->flags == NULL)
		printf("\n");
	exit(0);
}
