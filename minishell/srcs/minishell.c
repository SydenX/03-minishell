/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/06 14:31:54 by jtollena         ###   ########.fr       */
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

void	execute_cmd_setup(char **firstcmdcpy, t_cmd *cmd, char **path, char **cmdcpy)
{
	exit_code = 127;
	*firstcmdcpy = ft_strdup(cmd->cmd);
	if (*firstcmdcpy == NULL)
		exit(0);
	path = get_envpath();
	if (path == NULL)
	{
		free(*firstcmdcpy);
		exit(0);
	}
	*cmdcpy = ft_strdup(cmd->cmd);
	if (*cmdcpy == NULL)
	{
		free(*firstcmdcpy);
		free(path);
		exit(0);
	}
}

int	execute_cmd(t_cmd *cmd, char **envp)
{
	char	**path;
	int		i;
	char	*cmdcpy;
	char	*firstjoin;
	char	*firstcmdcpy;

	// if (cmd->cmd == "pwd")
	// 	pwd(cmd);
	// if (cmd->cmd == "cd")
	// 	cd(cmd);
	// if (cmd->cmd == "env")
	// 	env(cmd, envp);
	if (ft_strncmp(cmd->cmd, "echo", 4) == 0)
		echo(cmd, envp);
	else
	{
		execve(cmd->cmd, cmd->args, envp);
		i = 0;
		execute_cmd_setup(&firstcmdcpy, cmd, path, &cmdcpy);
		while (path[i] != NULL && exit_code == 127) 
		{
			firstjoin = ft_strjoin(path[i], "/");
			if (firstjoin == NULL)
			{
				free(firstcmdcpy);
				free(path);
				exit(0);
			}
			free(cmd->cmd);
			cmd->cmd = ft_strjoin(firstjoin, cmdcpy);
			free(firstjoin);
			if (cmd->cmd == NULL)
			{
				free(firstcmdcpy);
				free(path);
				exit(0);
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
	exit(127);
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

		if (ft_strncmp(cmd[i]->cmd, "exit", 4) == 0)
			return (exit_builtin(cmd[i]));
		else
		{
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
			}
			else {
				waitpid(pid, &code, 0);
				close(fd[1]);
				if (customoutput != -1)
					close(customoutput);
				if (custominput != -1)
					close(custominput);
				fdd = fd[0];
			}
		}
		i++;
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

		cmd1->cmd = ft_strdup("exit");
		cmd1->args = (char *[]){cmd1->cmd, "1", "5", "a", NULL};
		cmd1->flags = NULL;
		cmd1->output = NULL;
		cmd1->input = NULL;

		cmd2->cmd = ft_strdup("echo");
		cmd2->args = (char *[]){cmd2->cmd, "-->$?", NULL};
		cmd2->flags = NULL;
		cmd2->output = NULL;
		cmd2->input = NULL;

		cmd3->cmd = ft_strdup("cat");
		cmd3->args = (char *[]){cmd3->cmd, NULL};
		cmd3->flags = NULL;
		cmd3->output = NULL;
		cmd3->input = NULL;

		// HERE DOC TESTS
		char *newline;
		newline = ft_strdup("");
		int fd = open("tmp", O_WRONLY | O_CREAT | O_APPEND, 0666);
		cmd3->input = "tmp";
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
		
		//END OF HERE DOC TESTS

		// t_cmd *commands[] = {cmd1, NULL};
		// exit_code = execute_pipes(commands, envp);
		// t_cmd *commands1[] = {cmd2, NULL};
		// exit_code = execute_pipes(commands1, envp);
		t_cmd *commands2[] = {cmd3, NULL};
		exit_code = execute_pipes(commands2, envp);

		unlink("tmp"); // PART OF HERE DOC TESTS


		free(cmd1);
		free(cmd2);
		free(cmd3);
	}
	return (0);
}
