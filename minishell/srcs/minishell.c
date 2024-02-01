/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/01 15:08:17 by jtollena         ###   ########.fr       */
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

char	**get_envpath()
{
	char	*env = getenv("PATH");
	char	**path = ft_split(env, ':');
	return (path);
}

void	try_execution(char *line, char **envp)
{
	int	passed;
	int	i;
	char	*join;
	char	*firstjoin;
	char	**path;

	i = 0;
	exit_code = execve(ft_split(line, ' ')[0], &ft_split(line, ' ')[0], envp);
	path = get_envpath();
	if (path == NULL)
		return ;
	while (path[i] != NULL && exit_code == -1) 
	{
		firstjoin = ft_strjoin(path[i], "/");
		if (firstjoin == NULL)
			return ;
		join = ft_strjoin(firstjoin, ft_split(line, ' ')[0]);
		free(firstjoin);
		if (join == NULL)
			return ;
		exit_code = execve(join, &ft_split(line, ' ')[0], envp);
		if (exit_code != -1)
		{
			printf("*%d\n", exit_code);
			free(join);
			break ;
		}
		free(join);
		i++;
	}
	if (exit_code == -1)
	{
		exit_code = 1;
		printf("minishell: %s: command not found\n", ft_split(line, ' ')[0]);
	}
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
		else if (ft_strncmp(ft_split(line, ' ')[0], "env", 3) == 0)
			env(envp);
		else if (ft_strncmp(ft_split(line, ' ')[0], "exit", 4) == 0)
			exit_builtin(line);
		else if (ft_strncmp(ft_split(line, ' ')[0], "echo", 4) == 0)
			echo(&line[ft_strlen(ft_split(line, ' ')[0]) + 1], 0);
		else
		{	
			// pipe(fd);
			pid = fork();
			if (pid == -1)
				exit(1);
			if (pid == 0)
			{
				// dup2(fd[1], 0);
				// close(fd[0]);
				close(1);
				int fdd = open("testoutput", O_RDWR | O_CREAT);
				if (ft_split(line, ' ')[0] != NULL)
					try_execution(line, envp);
				// close(fd[1]);
				close(fdd);
				exit(1);
			}
			else
			{
				// close(fd[0]);
				// close(fd[1]);
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
	}
	return (0);
}
