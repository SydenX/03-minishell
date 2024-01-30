/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:26:48 by jtollena          #+#    #+#             */
/*   Updated: 2024/01/30 11:30:16 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != 0)
	{
		printf("%s=%s\n", envp[i] ,getenv(envp[i]));
		i++;
	}
}