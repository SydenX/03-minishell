/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:38:56 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/07 12:20:14 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(t_cmd *cmd, char **envp)
{
	if (cmd->args == NULL)
	{
		printf("unset: not enough arguments");
		exit(1);
	}
	exit(0);
}