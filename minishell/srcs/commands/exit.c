/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:31:45 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/07 12:57:53 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_string_digit(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		if ((str[i] < '0' || str[i] > '9'))
		{
			if (str[i] == '-' && (str[i + 1] < '0' || str[i + 1] > '9'))
				return (0);
			else if (str[i] != '-')
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_arguments(char **trimmed, t_cmd *cmd, int *code)
{
	if (cmd->args[2] != NULL)
	{
		printf("minishell: exit: too many arguments\n");
		return (free(*trimmed), 1);
	}
	printf("exit\n");
	*code = ft_atoi(*trimmed);
	free(*trimmed);
	exit(*code);
}

int	check_single_quoted(char **trimmed, t_cmd *cmd, int *code)
{
	free(*trimmed);
	*trimmed = ft_strtrim(cmd->args[1], "\'");
	if (*trimmed == NULL)
		exit(1);
	if (is_string_digit(*trimmed) == 1)
		if (check_arguments(trimmed, cmd, code))
			return (1);
	return (0);
}

int	exit_builtin(t_cmd *cmd)
{
	char	*trimmed;
	int		code;

	if (cmd->args[1] != NULL)
	{
		trimmed = ft_strtrim(cmd->args[1], "\"");
		if (trimmed == NULL)
			exit(1);
		if (is_string_digit(trimmed) == 1)
		{
			if (check_arguments(&trimmed, cmd, &code))
				return (1);
		}
		else if (cmd->args[1][0] == '\'' && cmd->args[1][ft_strlen(cmd->args[1]) - 1] == '\'')
			if (check_single_quoted(&trimmed, cmd, &code))
				return (1);
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", trimmed);
		free(trimmed);
		exit(255);
	}
	printf("exit\n");
	exit(exit_code);
}
