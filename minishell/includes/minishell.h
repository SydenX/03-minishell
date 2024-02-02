/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:21 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/02 12:40:55 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

int exit_code;

typedef struct	cmd {
	char	*input;
	char	*output;
	char	*cmd;
	char	**flags;
	char	**args;
}	t_cmd;

//Built-in cmds
void	pwd();
void	cd(char *arg);
void	env(char **envp);
void	exit_builtin(char *line);
void	echo(char *out, int has_param);

#endif
