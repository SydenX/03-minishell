/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/02 15:09:24 by jtollena         ###   ########.fr       */
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

int		execute(t_cmd cmd, char **env)
{
	int fd[2];
	int customoutput;
	pid_t pid;
	int	code;
	char	buf;
	
	pipe(fd);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (cmd.output != NULL)
		customoutput = open(cmd.output, O_WRONLY | O_CREAT);
	buf = '\n';
	if (pid == 0)
	{
		close(fd[1]);
		while (read(fd[0], &buf, 1) > 0)
			write(STDOUT_FILENO, &buf, 1);
		close(fd[0]);
		execve(cmd.cmd, cmd.args, env);
		exit(127);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
		if (cmd.output != NULL)
			close(customoutput);
		waitpid(pid, &code, 0);
		return (((code >> 8) & 0x000000ff));
	}
	return (127);
}

void	try_execution(char *line, char **envp)
{
	int	passed;
	int	i;
	char	*join;
	char	*firstjoin;
	char	**path;
	t_cmd	cmd;

	i = 0;
	cmd.cmd = ft_split(line, ' ')[0];
	if (cmd.cmd == NULL)
		return ;
	// cmd.input = ft_strdup("testinput");
	// cmd.output = ft_strdup("testoutput");
	cmd.input = NULL;
	cmd.output = NULL;
	cmd.args = &ft_split(line, ' ')[0];
	if (cmd.args == NULL)
	{
		free(cmd.cmd);
		return ;
	}
	exit_code = execute(cmd, envp);
	path = get_envpath();
	if (path == NULL)
	{
		free(cmd.cmd);
		free(cmd.args);
		return ;
	}
	char *cmdcpy = ft_strdup(cmd.cmd);
	if (cmdcpy == NULL)
	{
		free(cmd.cmd);
		free(cmd.args);
		return ;
	}
	while (path[i] != NULL && exit_code == 127) 
	{
		firstjoin = ft_strjoin(path[i], "/");
		if (firstjoin == NULL)
		{
			free(cmd.cmd);
			free(cmd.args);
			return ;
		}
		free(cmd.cmd);
		cmd.cmd = ft_strjoin(firstjoin, cmdcpy);
		free(firstjoin);
		if (cmd.cmd == NULL)
		{
			free(cmd.args);
			return ;
		}
		exit_code = execute(cmd, envp);
		if (exit_code != 127)
		{
			free(cmd.cmd);
			break ;
		}
		i++;
	}
	if (exit_code == 127)
	{
		exit_code = 127;
		printf("minishell: %s: command not found\n", ft_split(line, ' ')[0]);
	}
	free(cmd.args);
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
		else if (ft_split(line, ' ')[0] != NULL)
			try_execution(line, envp);
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
