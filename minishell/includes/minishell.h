/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:21 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 11:22:13 by jtollena         ###   ########.fr       */
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
# include <termios.h>

int exit_code;

typedef struct	s_cmd {
	char	*input;
	char	*output;
	char	*cmd;
	char	**flags;
	char	**args;
}	t_cmd;

//Built-in cmds
void	pwd(t_cmd *cmd);
void	cd(t_cmd *cmd);
void	env(t_cmd *cmd, char **envp);
void	exit_builtin(t_cmd *cmd);
void	echo(t_cmd *cmd, char **env);
int		replace(char **str, char to_replace, char replace_by);

#endif
