/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 13:13:40 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/23 13:08:40 by jtollena         ###   ########.fr       */
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
				return (NULL);
			if (cmd[i]->overrite_heredoc)
			{
				pid_t f = fork();
				if (f == 0)
				{
					while (1)
					{
						if (ft_strncmp(newline, cmd[i]->heredoc, ft_strlen(newline) + 1) != 0)
						{
							newline = readline("> ");
							if (newline == NULL)
								break ;
						}
						else
							break ;
					}
					exit(1);
				}
				else
					waitpid(f, NULL, 0);
				cmd[i]->has_heredoc = 0;
			}
			else
			{
				char *tmpname;
				tmpname = ft_strdup("./tmp/i");
				if (tmpname == NULL)
					return (free(newline), NULL);
				char *tmpaddon;
				tmpaddon = ft_strdup("i");
				if (tmpaddon == NULL)
					return (free(tmpname), free(newline), NULL);
				int j = 0;
				while (!access(tmpname, F_OK))
				{
					char *tmpnamecpy = ft_strdup(tmpname);
					free(tmpname);
					if (tmpnamecpy == NULL)
						return (free(newline), free(tmpaddon), NULL);
					tmpname = ft_strjoin(tmpnamecpy, tmpaddon);
					free(tmpnamecpy);
					if (tmpname == NULL)
						return (free(newline), free(tmpaddon) , NULL);
					j++;
				}
				int fd = open(tmpname, O_WRONLY | O_CREAT | O_APPEND, 0666);
				cmd[i]->input = tmpname;
				pid_t f = fork();
				if (f == 0)
				{
					while (1)
					{
						if (ft_strncmp(newline, cmd[i]->heredoc, ft_strlen(newline) + 1) != 0)
						{
							newline = readline("> ");
							if (newline == NULL)
								break ;
							else if (ft_strncmp(newline, cmd[i]->heredoc, ft_strlen(newline) + 1) != 0)
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
				free(tmpaddon);
			}
			free(newline);
		}
		i++;
	}
	return (cmd);
}
