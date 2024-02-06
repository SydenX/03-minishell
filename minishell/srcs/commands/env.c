/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:26:48 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 11:22:33 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(t_cmd *cmd, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != 0)
	{
		printf("%s=%s\n", envp[i] ,getenv(envp[i]));
		i++;
	}
	exit_code = 0;
}
