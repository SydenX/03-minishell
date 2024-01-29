/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/01/29 15:19:51 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrlc_handler()
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals()
{
	struct sigaction	sact;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &ctrlc_handler);
}

int	execute_cmd(char *line, char **envp)
{
	int fd[2];
	pid_t pid;
	char	*output;
	
	if (ft_split(line, ' ')[0] != NULL){
		if (ft_strncmp(ft_split(line, ' ')[0], "pwd", 3) == 0)
			pwd();
		else if (ft_strncmp(ft_split(line, ' ')[0], "cd", 2) == 0)
			cd(ft_split(line, ' ')[1]);
		else
		{
			pipe(fd);
			pid = fork();
			if (pid == -1)
				exit(1);
			if (pid == 0)
			{
				dup2(fd[1], 0);
				close(fd[0]);
				if (ft_split(line, ' ')[0] != NULL)
				{
					if (ft_strnstr(ft_split(line, ' ')[0], "/bin/", ft_strlen(ft_split(line, ' ')[0])))
					{
						if (execve(ft_split(line, ' ')[0], &ft_split(line, ' ')[0], envp) == -1)
							fprintf(stderr, "Failed to execute '%s'\n", ft_split(line, ' ')[0]);
					}
					else
						if (execve(ft_strjoin("/bin/", ft_split(line, ' ')[0]), &ft_split(line, ' ')[0], envp) == -1)
								fprintf(stderr, "Failed to execute '%s'\n", ft_split(line, ' ')[0]);
				}
				close(fd[1]);
				exit(1);
			}
			else
			{	
				close(fd[0]);
				close(fd[1]);
				waitpid(pid, NULL, 0);
			}
		}
	}
	return (1);
}

int	main(int argc, char *argv[], char **envp)
{
	setup_signals();
	while (1)
	{
		char	*line = readline("[minishell] ");
		if (line == NULL)
			break ;
		add_history(line);
		execute_cmd(line, envp);

		if (!ft_strncmp(line, "exit", 5))
			return (printf("exit\n"), 1);
	}
	return (0);
}
