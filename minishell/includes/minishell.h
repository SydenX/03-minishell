/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:21 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/08 16:38:42 by jtollena         ###   ########.fr       */
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

typedef struct	s_cmd {
	char	*input;
	char	*output;
	char	*cmd;
	char	**flags;
	char	**args;
	int		has_heredoc;
	t_type	next_element;
}	t_cmd;

typedef struct s_subshell {
	t_cmd		**cmds;
	t_type		next_element;
	int			has_heredoc;
}	t_subshell;


typedef struct	s_node {
	t_cmd	**cmd;
	t_type	type;
}	t_node;

//Built-in cmds
int		pwd(t_cmd *cmd, char **envp);
int		cd(t_cmd *cmd, char **envp);
int		env(t_cmd *cmd, char **envp);
int		unset(t_cmd *cmd, char **envp);
int		exit_builtin(t_cmd *cmd);
int		echo(t_cmd *cmd, char **env);
int		replace(char **str, char to_replace, char replace_by);

#endif
