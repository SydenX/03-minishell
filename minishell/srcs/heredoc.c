/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 13:13:40 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/09 13:14:00 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	**read_heredoc(t_cmd **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
	{
		if (cmd[i]->has_heredoc)
		{
			char *newline;
			newline = ft_strdup("");
			if (newline == NULL)
				exit(1);
			char *tmpname;
			tmpname = ft_strdup("./tmp/i");
			if (tmpname == NULL)
				return (free(newline), exit(1), cmd);
			char *tmpaddon;
			tmpaddon = ft_strdup("i");
			if (tmpname == NULL)
				return (free(tmpname), free(newline), exit(1), cmd);
			int j = 0;
			while (!access(tmpname, F_OK))
			{
				tmpname = ft_strjoin(tmpname, tmpaddon);
				if (tmpname == NULL)
					return (free(newline), exit(1), cmd);
				j++;
			}
			int fd = open(tmpname, O_WRONLY | O_CREAT | O_APPEND, 0666);
			cmd[i]->input = tmpname;
			pid_t f = fork();
			if (f == 0)
			{
				while (1)
				{
					if (ft_strncmp(newline, "eof", 3) != 0)
					{
						newline = readline("> ");
						if (newline == NULL)
							break ;
						else if (ft_strncmp(newline, "eof", 3) != 0)
						{
							write(fd, newline, ft_strlen(newline));
							write(fd, "\n", 1);
						}
					}
					else
						break ;
				}
				exit(1);
			}
			else
				waitpid(f, NULL, 0);
			close(fd);
		}
		i++;
	}
	return (cmd);
}

t_subshell	**read_sub_heredoc(t_subshell **subshell)
{
	int	i;

	i = 0;
	while (subshell[i] != NULL)
	{
		if (subshell[i]->has_heredoc)
		{
			char *newline;
			newline = ft_strdup("");
			if (newline == NULL)
				return(subshell);
			char *tmpname;
			tmpname = ft_strdup("./tmp/i");
			if (tmpname == NULL)
				return (free(newline), exit(1), subshell);
			char *tmpaddon;
			tmpaddon = ft_strdup("i");
			if (tmpname == NULL)
				return (free(tmpname), free(newline), exit(1), subshell);
			int j = 0;
			while (!access(tmpname, F_OK))
			{
				tmpname = ft_strjoin(tmpname, tmpaddon);
				if (tmpname == NULL)
					return (free(newline), exit(1), subshell);
				j++;
			}
			int fd = open(tmpname, O_WRONLY | O_CREAT | O_APPEND, 0666);
			subshell[i]->input = tmpname;
			j = 0;
			while (subshell[i]->cmds[j] != NULL){
				if (subshell[i]->cmds[j]->has_heredoc != 1 && subshell[i]->cmds[j]->input == NULL)
					subshell[i]->cmds[j]->input = tmpname;
				j++;
			}
			pid_t f = fork();
			if (f == 0)
			{
				while (1)
				{
					if (ft_strncmp(newline, "eof", 3) != 0)
					{
						newline = readline("> ");
						if (newline == NULL)
							break ;
						else if (ft_strncmp(newline, "eof", 3) != 0)
						{
							write(fd, newline, ft_strlen(newline));
							write(fd, "\n", 1);
						}
					}
					else
						break ;
				}
				exit(1);
			}
			else
				waitpid(f, NULL, 0);
			close(fd);
		}
		i++;
	}
	return (subshell);
}