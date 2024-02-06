/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:31:45 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 11:23:18 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	send_exit_code(int exitcode)
{	
	exit(exitcode);
}

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

void	exit_builtin(t_cmd *cmd)
{
	char	**splited;

	// splited = ft_split(line, ' ');
	// if (splited == NULL)
	// 	return ;
	// if (splited[1] != NULL)
	// {
	// 	if (is_string_digit(splited[1]) == 1)
	// 	{
	// 		if (splited[2] != NULL)
	// 		{
	// 			printf("minishell: exit: too many arguments\n");
	// 			exit_code = 1;
	// 			free(splited);
	// 			return ;
	// 		}
	// 		printf("exit %d\n", ft_atoi(splited[1]));
	// 		free(splited);
	// 		send_exit_code(ft_atoi(splited[1]));
	// 	}
	// 	printf("exit\n");
	// 	printf("minishell: exit: %s: numeric argument required\n", splited[1]);
	// 	free(splited);
	// 	send_exit_code(255);
	// }
	// printf("exit %d\n", 1);
	// free(splited);
	// send_exit_code(1);
}
