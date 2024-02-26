/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 13:13:40 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/26 14:40:12 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc(t_cmd *cmd, char *newline, pid_t f, int fd)
{
	if (f == 0)
	{
		while (1)
		{
			if (ft_strncmp(newline, cmd->heredoc, ft_strlen(newline) + 1) != 0)
			{
				newline = readline("> ");
				if (newline == NULL)
					break ;
				else if (ft_strncmp(newline, cmd->heredoc,
						ft_strlen(newline) + 1) != 0)
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
}

int	open_heredoc(t_cmd *cmd, char	*newline, char *tmpname, char *tmpaddon)
{
	int		j;
	int		fd;
	pid_t	f;
	char	*tmpnamecpy;

	j = 0;
	while (!access(tmpname, F_OK))
	{
		tmpnamecpy = ft_strdup(tmpname);
		free(tmpname);
		if (tmpnamecpy == NULL)
			return (free(newline), free(tmpaddon), 0);
		tmpname = ft_strjoin(tmpnamecpy, tmpaddon);
		free(tmpnamecpy);
		if (tmpname == NULL)
			return (free(newline), free(tmpaddon), 0);
		j++;
	}
	fd = open(tmpname, O_WRONLY | O_CREAT | O_APPEND, 0666);
	cmd->input = tmpname;
	f = fork();
	write_heredoc(cmd, newline, f, fd);
	close(fd);
	free(tmpaddon);
	return (1);
}

int	overrite_heredoc(t_cmd *cmd, char	*newline)
{
	pid_t	f;

	f = fork();
	if (f == 0)
	{
		while (1)
		{
			if (ft_strncmp(newline, cmd->heredoc, ft_strlen(newline) + 1) != 0)
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
	cmd->has_heredoc = 0;
	return (1);
}

int	check_for_overrite(t_cmd *cmd, char	*newline)
{
	char	*tmpaddon;
	char	*tmpname;

	if (cmd->overrite_heredoc)
		return (overrite_heredoc(cmd, newline));
	else
	{
		tmpname = ft_strdup("./tmp/i");
		if (tmpname == NULL)
			return (free(newline), 0);
		tmpaddon = ft_strdup("i");
		if (tmpaddon == NULL)
			return (free(tmpname), free(newline), 0);
		return (open_heredoc(cmd, newline, tmpname, tmpaddon));
	}
}

t_cmd	*read_single_heredoc(t_cmd *cmd)
{
	char	*newline;

	if (cmd->has_heredoc)
	{
		newline = ft_strdup("");
		if (newline == NULL)
			return (NULL);
		if (check_for_overrite(cmd, newline) == 0)
			return (NULL);
		free(newline);
	}
	return (cmd);
}

t_cmd	**read_heredoc(t_cmd **cmd)
{
	int		i;
	char	*newline;

	i = 0;
	while (cmd[i] != NULL)
	{
		if (cmd[i]->has_heredoc == 1)
		{
			newline = ft_strdup("");
			if (newline == NULL)
				return (NULL);
			if (check_for_overrite(cmd[i], newline) == 0)
				return (NULL);
			free(newline);
		}
		i++;
	}
	return (cmd);
}
