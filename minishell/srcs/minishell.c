/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/23 12:15:56 by jtollena         ###   ########.fr       */
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
	if (ft_strncmp(cmd->cmd, "pwd", ft_strlen(cmd->cmd)) == 0)
		return (pwd(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd)) == 0)
		return (cd(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd)) == 0)
		return (env(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd)) == 0)
		return (echo(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd)) == 0)
		return (unset(cmd, envp), 0);
	else if (ft_strncmp(cmd->cmd, "echo", ft_strlen(cmd->cmd)) == 0)
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
		free(cmdcpy);
		free(path);
	}
	exit(127);
}

int	execute_pipes(t_cmd **cmd, char **env, int	fd[2])
{
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
				custominput = open(cmd[i]->input, O_RDONLY | O_CREAT, 0666);
			if (i > 0 && custominput == -1)
				if (cmd[i - 1]->output != NULL)
					custominput = open(cmd[i - 1]->output, O_RDONLY | O_CREAT, 0666);
			if (cmd[i]->output != NULL && cmd[i]->output_type == TRUNC)
				customoutput = open(cmd[i]->output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (cmd[i]->output != NULL && cmd[i]->output_type == APPEND)
				customoutput = open(cmd[i]->output, O_WRONLY | O_CREAT | O_APPEND, 0666);
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

int	execute_cmds(t_cmd **cmd, char **env)
{
	int	i = 0;
	int j;
	int	fd[2];
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
			while (j + i < cmd_size(cmd))
			{
				if (!cmd[j + i - 1]->has_pipe)
					break ;
				next_pipe[j] = cmd[j + i];
				j++;
			}
			i += j;
			next_pipe[j] = NULL;
			exit_code = execute_pipes(next_pipe, env, fd);
		}
		else if (cmd[i]->previous_element == AND)
		{
			if (exit_code == 0)
			{
				next_pipe[0] = cmd[i];
				next_pipe[1] = NULL;
				exit_code = execute_pipes(next_pipe, env, fd);
			}
		}
		else if (cmd[i]->previous_element == OR)
		{
			if (exit_code > 0)
			{
				next_pipe[0] = cmd[i];
				next_pipe[1] = NULL;
				exit_code = execute_pipes(next_pipe, env, fd);
			}
		}
		else
		{
			next_pipe[0] = cmd[i];
			next_pipe[1] = NULL;
			exit_code = execute_pipes(next_pipe, env, fd);
		}
		i++;
	}
	return (free(next_pipe), 0);
}

void checkLeaks() {
	system("leaks minishell");
}

int	get_cmd_args_number(char **split, int j)
{
	int	current_args = 0;
	while (split[j] != NULL && ft_strncmp(split[j], "&&", 2) != 0 && ft_strncmp(split[j], "||", 2) != 0) {
		if (split[j][0] == '|' && ft_strlen(split[j]) == 1)
			return (current_args);
		else if (split[j][0] == '<' && ft_strlen(split[j]) == 1)
			return (current_args);
		else if (split[j][0] == '>' && ft_strlen(split[j]) == 1)
			return (current_args);
		else if (ft_strncmp(split[j], ">>", 2) == 0)
			return (current_args);
		else if (ft_strncmp(split[j], "<<", 2) == 0)
			return (current_args);
		current_args++;
		j++;
	}
	return (current_args);
}

int	get_cmd_number(char **split)
{
	int current_cmd = 0;
	int j = 0;
	while (split[j] != NULL) {
		int k = 0;
		while (split[j] != NULL && ft_strncmp(split[j], "&&", 2) != 0 && ft_strncmp(split[j], "||", 2) != 0) {
			if (split[j][0] == '|' && ft_strlen(split[j]) == 1) {
				j++;
				break ;
			} else if (split[j][0] == '<' && ft_strlen(split[j]) == 1)
				j++;
			else if (split[j][0] == '>' && ft_strlen(split[j]) == 1)
				j++;
			else if (ft_strncmp(split[j], ">>", 2) == 0)
				j++;
			else if (ft_strncmp(split[j], "<<", 2) == 0)
				j++;
			else
				j++;
		}
		current_cmd++;
		if (split[j] != NULL)
			if (ft_strncmp(split[j], "&&", 2) == 0 || ft_strncmp(split[j], "||", 2) == 0)
				j++;
	}
	return (current_cmd);
}

int	main(int argc, char *argv[], char **envp)
{
	struct termios term;
	
	setup_signals();

    tcgetattr(fileno(stdin), &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(fileno(stdin), 0, &term);
	int it = 0;
	while (it++ < 1)
	{
		checkLeaks();
		// char	*line = readline("\x1b[34;01m[MiniShell] \x1b[39;49;00m");
		char *line = ft_strdup("echo 1 && echo 3 && cat << e");
		if (line == NULL)
			break ;
		// add_history(line);

		char	**split = ft_split(line, ' ');
		int 	j = 0;
		int 	current_cmd = 0;
		int		subcount = 0;
		t_type prev;
		prev = NOTSET;
		int current_sub = 0;
		t_cmd	**cmds = malloc((get_cmd_number(split) + 1) * sizeof(t_cmd *));
		while (split[j] != NULL) {
			t_cmd *cmd = malloc(sizeof(t_cmd));
			cmd->cmd = ft_strdup(split[j]);
			cmd->args = malloc((get_cmd_args_number(split, j) + 1) * sizeof(char *));
			int k = 0;
			cmd->args[k++] = ft_strdup(split[j++]);
			while (split[j] != NULL && ft_strncmp(split[j], "&&", 2) != 0 && ft_strncmp(split[j], "||", 2) != 0) {
				if (split[j][0] == '|' && ft_strlen(split[j]) == 1) {
					cmd->has_pipe = 1;
					j++;
					break ;
				} else if (split[j][0] == '<' && ft_strlen(split[j]) == 1) {
					j++;
					cmd->input = split[j++];
				} else if (split[j][0] == '>' && ft_strlen(split[j]) == 1) {
					j++;
					cmd->output = split[j++];
					cmd->output_type = TRUNC;
				} else if (ft_strncmp(split[j], ">>", 2) == 0) {
					j++;
					cmd->output = split[j++];
					cmd->output_type = APPEND;
				} else if (ft_strncmp(split[j], "<<", 2) == 0) {
					j++;
					cmd->heredoc = split[j++];
					cmd->has_heredoc = 1;
				} else
					cmd->args[k++] = ft_strdup(split[j++]); // Copier les arguments suivants
			}
			cmd->previous_element = prev;
			cmd->args[k] = NULL; // Terminer le tableau d'arguments
			cmds[current_cmd++] = cmd;
			if (split[j] != NULL)
			{
				if (ft_strncmp(split[j], "&&", 2) == 0 || ft_strncmp(split[j], "||", 2) == 0)
				{
					if(ft_strncmp(split[j], "&&", 2) == 0)
						prev = AND;
					else if(ft_strncmp(split[j], "||", 2) == 0)
						prev = OR;
					else
						prev = NOTSET;
					j++;
				}
				else
					prev = NOTSET;
			}
		}
		cmds[current_cmd] = NULL;
		execute_cmds(cmds, envp);
		int tt = 0;
		while (split[tt] != NULL)
			free(split[tt++]);
		free(split);
		tt = 0;
		while (cmds[tt] != NULL)
		{
			int ttt = 0;
			while (cmds[tt]->args[ttt] != NULL)
				free(cmds[tt]->args[ttt++]);
			free(cmds[tt]->cmd);
			free(cmds[tt++]);
		}
		free(cmds);
		free(line);
	}
	return (0);
}
