/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:21 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/23 13:02:15 by jtollena         ###   ########.fr       */
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

typedef enum	e_type {
	PIPE,
	AND,
	OR,
	NOTSET
}	t_type;

typedef enum	e_output_type {
	APPEND,
	TRUNC
}	t_output_type;

typedef struct	s_cmd {
	char				*input;
	char				*output;
	char				*cmd;
	char				**flags;
	char				**args;
	int					has_heredoc;
	int					overrite_heredoc;
	char				*heredoc;
	int					has_pipe;
	t_output_type		output_type;
	t_type				previous_element;
}	t_cmd;

//Built-in cmds
int		pwd(t_cmd *cmd, char **envp);
int		cd(t_cmd *cmd, char **envp);
int		env(t_cmd *cmd, char **envp);
int		unset(t_cmd *cmd, char **envp);
int		exit_builtin(t_cmd *cmd);
int		echo(t_cmd *cmd, char **env);
int		replace(char **str, char to_replace, char replace_by);

//Heredoc
t_cmd	**read_heredoc(t_cmd **cmd);

#endif
