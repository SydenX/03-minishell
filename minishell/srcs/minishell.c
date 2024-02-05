/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/05 15:52:00 by jtollena         ###   ########.fr       */
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

int	execute_pipes(t_cmd **cmd, char **env)
{
	int fd[2];
	int	i;
	pid_t pid;
	int fdd = 0;
	int	code;
	int customoutput;
	int custominput;

	i = 0;
	while (cmd[i] != NULL) {
		customoutput = -1;
		custominput = -1;

		pipe(fd);
		if (cmd[i]->input != NULL)
			custominput = open(cmd[i]->input, O_RDONLY | O_CREAT);
		if (i > 0 && custominput == -1)
			if (cmd[i - 1]->output != NULL)
				custominput = open(cmd[i - 1]->output, O_RDONLY | O_CREAT);
		if (cmd[i]->output != NULL)
			customoutput = open(cmd[i]->output, O_WRONLY | O_CREAT | O_TRUNC);
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) {
			if (custominput != -1)
				dup2(custominput, STDIN_FILENO);
			else
				dup2(fdd, STDIN_FILENO);
			if (cmd[i + 1] != NULL) {
				dup2(fd[1], STDOUT_FILENO);
			}
			if (customoutput != -1)
				dup2(customoutput, STDOUT_FILENO);
			close(fd[0]);
			execve(cmd[i]->cmd, cmd[i]->args, env);
			exit(127);
		}
		else {
			waitpid(pid, &code, 0);
			close(fd[1]);
			if (customoutput != -1)
				close(customoutput);
			if (custominput != -1)
				close(custominput);
			fdd = fd[0];
			i++;
		}
	}
	return (((code >> 8) & 0x000000ff));
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
	
	setup_signals();
	while (1)
	{
		char	*line = readline("[minishell] ");
		if (line == NULL)
			break ;
		add_history(line);
		// execute_cmd(line, envp);

		t_cmd *cmd1, *cmd2, *cmd3;
		cmd1 = malloc(sizeof(t_cmd));
		cmd2 = malloc(sizeof(t_cmd));
		cmd3 = malloc(sizeof(t_cmd));

		cmd1->cmd = "/bin/cat";
		cmd1->args = (char *[]){"/bin/cat", "Makefile", NULL};
		cmd1->output = NULL;
		cmd1->input = NULL;

		cmd2->cmd = "/usr/bin/wc";
		cmd2->args = (char *[]){"/usr/bin/wc", "-l", NULL};
		cmd2->output = "testinput";
		cmd2->input = NULL;

		cmd3->cmd = "/usr/bin/wc";
		cmd3->args = (char *[]){"/usr/bin/wc", NULL};
		cmd3->output = "testoutput";
		cmd3->input = "testinput";

		t_cmd *commands[] = {cmd1, cmd2, cmd3, NULL};
		execute_pipes(commands, envp);
		// execute_pipes(commands, 3, envp);
	}
	return (0);
}
