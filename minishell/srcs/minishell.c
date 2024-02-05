/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/05 13:54:25 by jtollena         ###   ########.fr       */
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

int	execute_pipes(t_cmd *cmds, char **env)
{
	int	fd[6];
	int	i = 0;
	while (i < 3)
	{
		pipe(fd + i*2);
		i++;
	}
	i = 0;
	int j = 0;
	while (i < 3)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			if (i != 0)
				dup2(fd[j - 2], STDIN_FILENO);
			if (i != 2)
				dup2(fd[j + 1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			close(fd[2]);
			close(fd[3]);
			close(fd[4]);
			close(fd[5]);
			execve(cmds[i].cmd, cmds[i].args, env);
			exit(127);
		}
		j+=2;
		i++;
	}
	i = 0;
	while (i < 6)
	{
		close(fd[i]);
		i++;
	}
	i = 0;
	int code;
	while (i < 3)
	{
		wait(&code);
		i++;
	}
	return (127);
}

int	execute_pipe(t_cmd first, t_cmd second, char **env)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int	code;
	int customoutput;

	pipe(fd);
	pid1 = fork();
	if (pid1 == -1)
		exit(1);
	if (pid1 == 0)
	{
		pid2 = fork();
		if (pid2 == -1)
			exit(1);
		if (second.output != NULL)
			customoutput = open(second.output, O_WRONLY | O_CREAT | O_TRUNC);
		if (pid2 == 0)
		{
			dup2(fd[1], STDOUT_FILENO);
			execve(first.cmd, first.args, env);
			close(fd[0]);
			exit(127);
		} else {
			waitpid(pid2, &code, 0);

			dup2(fd[0], STDIN_FILENO);
			close(fd[1]);
			dup2(fd[1], STDOUT_FILENO);
			execve(second.cmd, second.args, env);
			exit(127);
		}
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
		if (second.output != NULL)
			close(customoutput);
		waitpid(pid1, &code, 0);
		waitpid(pid2, &code, 0);
		return (((code >> 8) & 0x000000ff));
	}
	return (127);
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
	customoutput = 1;
	if (cmd.output != NULL)
		customoutput = open(cmd.output, O_WRONLY | O_CREAT | O_TRUNC);
	buf = '\n';
	while (read(customoutput, &buf, 1) > 0)
		write(customoutput, 0, 1);
	if (pid == 0)
	{
		close(fd[1]);
		if (cmd.output != NULL)
			dup2(customoutput, STDOUT_FILENO);
		else
			dup2(fd[1], STDOUT_FILENO);
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
	cmd.output = ft_strdup("testoutput");
	cmd.input = NULL;
	// cmd.output = NULL;
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
			echo(&ft_split(line, ' ')[1], 0, envp);
		else if (ft_split(line, ' ')[0] != NULL)
			try_execution(line, envp);
	}
	return (1);
}

int	main(int argc, char *argv[], char **envp)
{
	t_cmd cmd1;
	t_cmd cmd2;
	t_cmd cmd3;
	t_cmd *cmds;
	
	setup_signals();
	while (1)
	{
		char	*line = readline("[minishell] ");
		if (line == NULL)
			break ;
		add_history(line);
		// execute_cmd(line, envp);

		cmd1.cmd = ft_strdup("/usr/bin/env");
		cmd2.cmd = ft_strdup("/usr/bin/wc");
		cmd3.cmd = ft_strdup("/usr/bin/grep");
		cmd2.output = NULL;
		cmd1.args = &line;
		cmd2.args = &line;
		cmd3.args = &line;
		cmds = malloc(sizeof(t_cmd) * 3);
		cmds[0] = cmd1;
		cmds[1] = cmd2;
		cmds[2] = cmd3;
		execute_pipes(cmds, envp);
	}
	return (0);
}
