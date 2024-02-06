/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 12:01:19 by jtollena         ###   ########.fr       */
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

int	execute_cmd(t_cmd *cmd, char **envp)
{
	char	**path;

	// if (cmd->cmd == "pwd")
	// 	pwd(cmd);
	// if (cmd->cmd == "cd")
	// 	cd(cmd);
	// if (cmd->cmd == "env")
	// 	env(cmd, envp);
	// if (cmd->cmd == "exit")
	// 	exit_builtin(cmd);
	if (ft_strncmp(cmd->cmd, "echo", 4) == 0)
		echo(cmd, envp);
	else
	{
		execve(cmd->cmd, cmd->args, envp);
		exit_code = 127;
		char	*firstcmdcpy;
		firstcmdcpy = ft_strdup(cmd->cmd);
		if (firstcmdcpy == NULL)
			return (0);
		int i = 0;
		char *firstjoin;
		path = get_envpath();
		if (path == NULL)
		{
			free(firstcmdcpy);
			return (0);
		}
		char *cmdcpy = ft_strdup(cmd->cmd);
		if (cmdcpy == NULL)
		{
			free(firstcmdcpy);
			free(path);
			return (0);
		}
		while (path[i] != NULL && exit_code == 127) 
		{
			firstjoin = ft_strjoin(path[i], "/");
			if (firstjoin == NULL)
			{
				free(firstcmdcpy);
				free(path);
				return (0);
			}
			free(cmd->cmd);
			cmd->cmd = ft_strjoin(firstjoin, cmdcpy);
			free(firstjoin);
			if (cmd->cmd == NULL)
			{
				free(firstcmdcpy);
				free(path);
				return (0);
			}
			execve(cmd->cmd, cmd->args, envp);
			exit_code = 127;
			i++;
		}
		if (exit_code == 127)
		{
			exit_code = 127;
			printf("minishell: %s: command not found\n", firstcmdcpy);
		}
		free(firstcmdcpy);
		free(path);
	}
	return (1);
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
			execute_cmd(cmd[i], env);
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

int	main(int argc, char *argv[], char **envp)
{
	struct termios term;
	t_cmd cmd1;
	t_cmd cmd2;
	t_cmd cmd3;
	
	setup_signals();

    tcgetattr(fileno(stdin), &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(fileno(stdin), 0, &term);

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

		cmd1->cmd = ft_strdup("cat");
		cmd1->args = (char *[]){cmd1->cmd, "o.txt", NULL};
		cmd1->output = NULL;
		cmd1->input = NULL;

		cmd2->cmd = ft_strdup("/bincat");
		cmd2->args = (char *[]){"/bin/cat", "-e", NULL};
		cmd2->output = NULL;
		cmd2->input = NULL;

		cmd3->cmd = ft_strdup("echo");
		cmd3->args = (char *[]){"echo", "hellow $?Cava", NULL};
		cmd3->output = NULL;
		cmd3->input = NULL;

		t_cmd *commands[] = {cmd1, cmd2, NULL};
		exit_code = execute_pipes(commands, envp);
		t_cmd *commands2[] = {cmd3, NULL};
		exit_code = execute_pipes(commands2, envp);
		free(cmd1);
		free(cmd2);
		free(cmd3);
		// execute_pipes(commands, 3, envp);
	}
	return (0);
}
