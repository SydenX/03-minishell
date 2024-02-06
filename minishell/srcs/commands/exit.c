/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:31:45 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 13:44:05 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_string_digit(char *str)
{
	while(*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	exit_builtin(t_cmd *cmd)
{
	char	*trimmed;

	if (cmd->args[1] != NULL)
	{
		trimmed = ft_strtrim(cmd->args[1], "\"");
		if (trimmed == NULL)
			exit(1);
		if (is_string_digit(trimmed) == 1)
		{
			if (cmd->args[2] != NULL)
			{
				printf("minishell: exit: too many arguments\n");
				return (free(trimmed), 1);
			}
			printf("exit\n");
			exit(ft_atoi(trimmed));
		}
		else if (cmd->args[1][0] == '\'' && cmd->args[1][ft_strlen(cmd->args[1]) - 1] == '\'')
		{
			free(trimmed);
			trimmed = ft_strtrim(cmd->args[1], "\'");
			if (trimmed == NULL)
				exit(1);
			if (is_string_digit(trimmed) == 1)
			{
				if (cmd->args[2] != NULL)
				{
					printf("minishell: exit: too many arguments\n");
					return (free(trimmed), 1);
				}
				printf("exit\n");
				exit(ft_atoi(trimmed));
			}
		}
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", trimmed);
		free(trimmed);
		exit(255);
	}
	printf("exit\n");
	exit(1);
}
