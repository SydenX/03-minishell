/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:31:45 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/01 13:47:48 by jtollena         ###   ########.fr       */
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

void	exit_builtin(char *line)
{
	if (ft_split(line, ' ')[1] != NULL)
	{
		if (is_string_digit(ft_split(line, ' ')[1]) == 1)
		{
			if (ft_split(line, ' ')[2] != NULL)
			{
				printf("minishell: exit: too many arguments\n");
				exit_code = 1;
				return ;
			}
			printf("exit %d\n", ft_atoi(ft_split(line, ' ')[1]));
			send_exit_code(ft_atoi(ft_split(line, ' ')[1]));
		}
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", ft_split(line, ' ')[1]);
		send_exit_code(255);
	}
	printf("exit %d\n", 1);
	send_exit_code(1);
}