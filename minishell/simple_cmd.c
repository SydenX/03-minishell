/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 13:04:25 by jtollena          #+#    #+#             */
/*   Updated: 2024/01/26 13:56:46 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	execute_simple_cmd(char *cmd, char **argv)
{
	execve(cmd, argv, NULL);
	return 0;
}

int	main(int argc, char **argv)
{
	execute_simple_cmd(argv[1], &argv[1]);
	return 0;
}
