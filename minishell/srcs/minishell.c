/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/09 15:35:41 by jtollena         ###   ########.fr       */
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

void	execute_cmd_setup(char **firstcmdcpy, t_cmd *cmd, char ***path, char **cmdcpy)
{
	exit_code = 127;
	*firstcmdcpy = ft_strdup(cmd->cmd);
	if (*firstcmdcpy == NULL)
		return ;
	*path = get_envpath();
	if (path == NULL)
	{
		free(firstcmdcpy);
		return ;
	}
	*cmdcpy = ft_strdup(cmd->cmd);
	if (*cmdcpy == NULL)
	{
		free(firstcmdcpy);
		free(path);
		return ;
	}
}

int	check_builtins(t_cmd *cmd, char **envp)
{
	if (ft_strncmp(cmd->cmd, "pwd", 3) == 0)
		return (pwd(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "cd", 2) == 0)
		return (cd(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "env", 3) == 0)
		return (env(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "export", 6) == 0)
		return (echo(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "unset", 5) == 0)
		return (unset(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "echo", 4) == 0)
		return (echo(cmd, envp), 0);
	else
		return (1);
}

int	execute_cmd(t_cmd *cmd, char **envp)
{
	char	**path;
	int		i;
	char	*cmdcpy;
	char	*firstjoin;
	char	*firstcmdcpy;

	if (check_builtins(cmd, envp))
	{
		execve(cmd->cmd, cmd->args, envp);
		i = 0;
		execute_cmd_setup(&firstcmdcpy, cmd, &path, &cmdcpy);
		while (path[i] != NULL && exit_code == 127) 
		{
			firstjoin = ft_strjoin(path[i], "/");
			if (firstjoin == NULL)
			{
				free(firstcmdcpy);
				free(path);
				return (1);
			}
			free(cmd->cmd);
			cmd->cmd = ft_strjoin(firstjoin, cmdcpy);
			free(firstjoin);
			if (cmd->cmd == NULL)
			{
				free(firstcmdcpy);
				free(path);
				return (1);
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

int	execute_pipes(t_cmd **cmd, char **env, t_subshell *subshell)
{
	// int fd[2];
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
			if (subshell->fd == NULL)
				subshell->fd = malloc(sizeof(int) * 2);
			if (subshell->fd == NULL)
				return (1);
			pipe(subshell->fd);
			if (cmd[i]->input != NULL)
				custominput = open(cmd[i]->input, O_RDONLY | O_CREAT);
			if (i > 0 && custominput == -1)
				if (cmd[i - 1]->output != NULL)
					custominput = open(cmd[i - 1]->output, O_RDONLY | O_CREAT);
			if (cmd[i]->output != NULL)
				customoutput = open(cmd[i]->output, O_WRONLY | O_CREAT | O_TRUNC);
			if ((pid = fork()) == -1) {
				perror("fork");
				return (1);
			}
			else if (pid == 0) {
				if (custominput != -1)
					dup2(custominput, STDIN_FILENO);
				else
					dup2(fdd, STDIN_FILENO);
				if (cmd[i + 1] != NULL) {
					dup2(subshell->fd[1], STDOUT_FILENO);
				}
				if (customoutput != -1)
					dup2(customoutput, STDOUT_FILENO);
				close(subshell->fd[0]);
				execute_cmd(cmd[i], env);
			}
			else {
				waitpid(pid, &code, 0);
				close(subshell->fd[1]);
				if (customoutput != -1)
					close(customoutput);
				if (custominput != -1)
					close(custominput);
				fdd = subshell->fd[0];
			}
			if (cmd[i]->has_heredoc)
				unlink(cmd[i]->input);
		}
		i++;
	}
	return (((code >> 8) & 0x000000ff));
}

int	cmd_size(t_cmd **cmd)
{
	int i = 0;
	while (cmd[i] != NULL)
		i++;
	return (i);
}

int	subshell_size(t_subshell **subshell)
{
	int i = 0;
	while (subshell[i] != NULL)
		i++;
	return (i);
}

int	execute_cmds(t_cmd **cmd, char **env, t_subshell *subshell)
{
	int	i = 0;
	int j;
	t_cmd	**next_pipe;

	next_pipe = malloc(cmd_size(cmd) * sizeof(t_cmd *));
	if (next_pipe == NULL)
		return (1);
	while (cmd[i] != NULL)
	{
		if (cmd[i]->has_pipe)
		{
			next_pipe[0] = cmd[i];
			j = 1;
			while (j + i < cmd_size(cmd) && cmd[j + i - 1]->has_pipe)
			{
				next_pipe[j] = cmd[j + i];
				j++;
			}
			i += j - 1;
			next_pipe[j] = NULL;
			j = 0;
			exit_code = execute_pipes(next_pipe, env, subshell);
		}
		else if (cmd[i]->previous_element == AND)
		{
			if (exit_code == 0)
			{
				next_pipe[0] = cmd[i];
				next_pipe[1] = NULL;
				exit_code = execute_pipes(next_pipe, env, subshell);
			}
		}
		else if (cmd[i]->previous_element == OR)
		{
			if (exit_code > 0)
			{
				next_pipe[0] = cmd[i];
				next_pipe[1] = NULL;
				exit_code = execute_pipes(next_pipe, env, subshell);
			}
		}
		else
		{
			next_pipe[0] = cmd[i];
			next_pipe[1] = NULL;
			exit_code = execute_pipes(next_pipe, env, subshell);
		}
		i++;
	}
	if (subshell->has_heredoc)
		unlink(subshell->input);
	return (free(next_pipe), 0);
}

int	execute_subshell_pipes(char **env, t_subshell **subshell)
{
	int fd[2];
	int	i;
	pid_t pid;
	int fdd = 0;
	int	code;
	int customoutput;
	int custominput;

	i = 0;
	while (subshell[i] != NULL) {
		customoutput = -1;
		custominput = -1;
		pipe(fd);
		if (subshell[i]->input != NULL)
			custominput = open(subshell[i]->input, O_RDONLY | O_CREAT);
		if (i > 0 && custominput == -1)
			if (subshell[i - 1]->output != NULL)
				custominput = open(subshell[i - 1]->output, O_RDONLY | O_CREAT);
		if (subshell[i]->output != NULL)
			customoutput = open(subshell[i]->output, O_WRONLY | O_CREAT | O_TRUNC);
		if ((pid = fork()) == -1) {
			perror("fork");
			return (1);
		}
		else if (pid == 0) {
			if (custominput != -1)
				dup2(custominput, STDIN_FILENO);
			else
				dup2(fdd, STDIN_FILENO);
			if (subshell[i + 1] != NULL) {
				dup2(fd[1], STDOUT_FILENO);
			}
			if (customoutput != -1)
				dup2(customoutput, STDOUT_FILENO);
			close(fd[0]);
			subshell[i]->fd = fd;
			exit_code = execute_cmds(subshell[i]->cmds, env, subshell[i]);
			exit(exit_code);
		}
		else 
		{
			waitpid(pid, &code, 0);
			close(fd[1]);
			if (customoutput != -1)
				close(customoutput);
			if (custominput != -1)
				close(custominput);
			fdd = fd[0];
		}
		i++;
	}
	return (((code >> 8) & 0x000000ff));
}

int	execute_subshell(t_subshell **subshell, char **env)
{
	int	i = 0;
	int j;
	t_subshell	**next_pipe;

	while (subshell[i] != NULL)
	{
		subshell[i]->cmds = read_heredoc(subshell[i]->cmds);
		i++;
	}
	i = 0;
	subshell = read_sub_heredoc(subshell);
	next_pipe = malloc(subshell_size(subshell) * sizeof(t_subshell *));
	if (next_pipe == NULL)
		return (1);
	while (subshell[i] != NULL)
	{
		if (subshell[i]->previous_element == AND)
		{
			if (exit_code == 0)
				execute_cmds(subshell[i]->cmds, env, subshell[i]);
		}
		else if (subshell[i]->previous_element == OR)
		{
			if (exit_code > 0)
				execute_cmds(subshell[i]->cmds, env, subshell[i]);
		}
		else if (subshell[i]->has_pipe)
		{
			next_pipe[0] = subshell[i];
			j = 1;
			while (j + i < subshell_size(subshell) && subshell[j + i - 1]->has_pipe)
			{
				next_pipe[j] = subshell[j + i];
				j++;
			}
			i += j - 1;
			next_pipe[j] = NULL;
			exit_code = execute_subshell_pipes(env, next_pipe);
		}
		else
			execute_cmds(subshell[i]->cmds, env, subshell[i]);
		i++;
	}
	return (free(next_pipe), 0);
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
		char	*line = readline("\x1b[34;01m[MiniShell] \x1b[39;49;00m");
		if (line == NULL)
			break ;
		add_history(line);

		// char	**split = ft_split(line, ' ');
		int j = 0;
		int l = 0;
		t_subshell *sub1, *sub2;
		// sub1 = malloc(sizeof(t_subshell));
		// sub1->cmds = malloc(50 * sizeof(t_cmd *));
		// while (split[j] != 0)
		// {
		// 	t_cmd *cmd;
		// 	cmd = malloc(sizeof(t_cmd));
		// 	cmd->args = malloc(100 * sizeof(char *));
		// 	cmd->previous_element = NOTSET;
		// 	if (j > 0)
		// 	{
		// 		if(ft_strncmp(split[j], "&&", 2) == 0)
		// 		{
		// 			j++;
		// 			cmd->previous_element = AND;
		// 		}
		// 		if(ft_strncmp(split[j], "||", 2) == 0)
		// 		{
		// 			j++;
		// 			cmd->previous_element = OR;
		// 		}
		// 	}
		// 	if (split[j] == 0)
		// 		break ;
		// 	cmd->cmd = split[j++];
		// 	int k = 0;
		// 	cmd->args[k++] = cmd->cmd;
		// 	while(split[j] != 0 && ft_strncmp(split[j], "&&", 2) != 0 && ft_strncmp(split[j], "||", 2) != 0)
		// 	{
		// 		if (split[j][0] == '|' && ft_strlen(split[j]) == 1)
		// 		{
		// 			cmd->has_pipe = 1;
		// 			j++;
		// 			break;
		// 		}
		// 		else
		// 			cmd->args[k++] = split[j++];
		// 	}
		// 	cmd->args[k] = NULL;
		// 	cmd->has_heredoc = 0;
		// 	cmd->flags = NULL;
		// 	cmd->output = NULL;
		// 	cmd->input = NULL;
		// 	cmd->previous_element = NOTSET;
		// 	sub1->cmds[l++] = cmd;
		// 	if (split[j] == 0)
		// 		break ;
		// 	// free(cmd);
		// }
		// sub1->cmds[l] = NULL;
		// sub1->has_heredoc = 0;
		t_cmd *cmd1, *cmd2, *cmd3, *cmd4, *cmd5;
		cmd1 = malloc(sizeof(t_cmd *));
		cmd2 = malloc(sizeof(t_cmd *));
		// cmd3 = malloc(sizeof(t_cmd));
		// cmd4 = malloc(sizeof(t_cmd));
		// cmd5 = malloc(sizeof(t_cmd));

		sub2 = malloc(sizeof(t_subshell *));
		// sub3 = malloc(sizeof(t_subshell));

		cmd1->cmd = ft_strdup("echo");
		cmd1->args = (char *[]){cmd1->cmd, "1", NULL};
		cmd1->flags = NULL;
		cmd1->output = NULL;
		cmd1->input = NULL;
		cmd1->previous_element = NOTSET;
		cmd1->has_pipe = 1;
		// cmd1->has_heredoc = 1;

		cmd2->cmd = ft_strdup("wc");
		cmd2->args = (char *[]){cmd2->cmd, NULL};
		cmd2->flags = NULL;
		cmd2->output = NULL;
		cmd2->input = NULL;
		cmd2->previous_element = NOTSET;
		// // cmd2->has_heredoc = 1;

		// cmd3->cmd = ft_strdup("echo");
		// cmd3->args = (char *[]){cmd3->cmd, "3", NULL};
		// cmd3->flags = NULL;
		// cmd3->output = NULL;
		// cmd3->input = NULL;
		// cmd3->previous_element = NOTSET;

		// cmd4->cmd = ft_strdup("echo");
		// cmd4->args = (char *[]){cmd4->cmd, "4", NULL};
		// cmd4->flags = NULL;
		// cmd4->output = NULL;
		// cmd4->input = NULL;
		// cmd4->previous_element = AND;

		// cmd5->cmd = ft_strdup("wc");
		// cmd5->args = (char *[]){cmd5->cmd, NULL};
		// cmd5->flags = NULL;
		// cmd5->output = NULL;
		// cmd5->input = NULL;
		// cmd5->previous_element = NOTSET;

		// // t_cmd *commands1[] = {cmd2, cmd3, NULL};
		// // exit_code = execute_subshell(commands1, envp);
		// t_cmd *commands[] = {cmd1, cmd2, NULL};
		t_cmd *commands2[] = {cmd1, cmd2, NULL};
		// t_cmd *commands3[] = {cmd5, NULL};

		// sub1->cmds = commands;
		// sub1->output = NULL;
		// sub1->input = NULL;

		sub2->cmds = commands2;
		sub2->previous_element = NOTSET;
		sub2->output = NULL;
		sub2->input = NULL;
		
		// sub3->cmds = commands3;
		// sub3->previous_element = OR;
		// sub3->output = NULL;
		// sub3->input = NULL;

		t_subshell **subs;
		subs = malloc(sizeof(t_subshell *) * 2);
		// subs[0] = sub1;
		subs[0] = sub2;
		// subs[2] = sub3;
		subs[1] = NULL;
		execute_subshell(subs, envp);
		// free(sub1->cmds);
		// free(sub1);
		// free(subs);
		// free(cmd1);
		// free(cmd2);
		// free(cmd3);
		// free(cmd4);
		// free(cmd5);
		// free(sub1);
		// free(sub2);
		// free(sub3);
		// free(subs);
	}
	return (0);
}
