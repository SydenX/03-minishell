/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:58:00 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/12 13:39:14 by jtollena         ###   ########.fr       */
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
		free(cmdcpy);
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
			pipe(subshell->fd);
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
			while (j + i < cmd_size(cmd))
			{
				if (!cmd[j + i - 1]->has_pipe)
					break ;
				next_pipe[j] = cmd[j + i];
				j++;
			}
			i += j;
			next_pipe[j] = NULL;
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
		pipe(subshell[i]->fd);
		if (subshell[i]->input != NULL)
			custominput = open(subshell[i]->input, O_RDONLY | O_CREAT, 0666);
		if (i > 0 && custominput == -1)
			if (subshell[i - 1]->output != NULL)
				custominput = open(subshell[i - 1]->output, O_RDONLY | O_CREAT, 0666);
		if (subshell[i]->output != NULL)
			customoutput = open(subshell[i]->output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
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
				dup2(subshell[i]->fd[1], STDOUT_FILENO);
			}
			if (customoutput != -1)
				dup2(customoutput, STDOUT_FILENO);
			close(subshell[i]->fd[0]);
			exit_code = execute_cmds(subshell[i]->cmds, env, subshell[i]);
			exit(exit_code);
		}
		else 
		{
			waitpid(pid, &code, 0);
			close(subshell[i]->fd[1]);
			if (customoutput != -1)
				close(customoutput);
			if (custominput != -1)
				close(custominput);
			fdd = subshell[i]->fd[0];
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
			{
				next_pipe[0] = subshell[i];
				next_pipe[1] = 0;
				exit_code = execute_subshell_pipes(env, next_pipe);
			}
		}
		else if (subshell[i]->previous_element == OR)
		{
			if (exit_code > 0)
			{
				next_pipe[0] = subshell[i];
				next_pipe[1] = 0;
				exit_code = execute_subshell_pipes(env, next_pipe);
			}
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
		{
			next_pipe[0] = subshell[i];
			next_pipe[1] = 0;
			exit_code = execute_subshell_pipes(env, next_pipe);
		}
		i++;
	}
	return (free(next_pipe), 0);
}

void checkLeaks() {
	system("leaks minishell");
}

void print_command_names(t_subshell **subs) {
    // Parcourir tous les sous-shells jusqu'à trouver NULL
    for (int i = 0; subs[i] != NULL; i++) {
        printf("Sous-shell %d :\n", i + 1);
        
        // Parcourir toutes les commandes dans ce sous-shell
        for (int j = 0; subs[i]->cmds[j] != NULL; j++) {
            printf("  Commande %d : %s\n", j + 1, subs[i]->cmds[j]->cmd);
        }
    }
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
	// atexit(checkLeaks);
	while (1)
	{
		char	*line = readline("\x1b[34;01m[MiniShell] \x1b[39;49;00m");
		if (line == NULL)
			break ;
		add_history(line);

		char	**split = ft_split(line, ' ');
		int j = 0;
		int l = 0;
		int	subcount = 0;
		
		t_subshell **subs;
		subs = malloc(sizeof(t_subshell *) * 50);

		t_type prev;
		prev = NOTSET;
		int current_sub = 0;
		subs[current_sub] = malloc(sizeof(t_subshell)); // Correction de l'allocation
		subs[current_sub]->cmds = malloc(50 * sizeof(t_cmd)); // Correction de l'allocation
		while (split[j] != NULL) { // Utilisation de NULL au lieu de 0
			if (split[j][0] == '(')
			{
				current_sub++;
				subs[current_sub] = malloc(sizeof(t_subshell)); // Correction de l'allocation
				subs[current_sub]->cmds = malloc(50 * sizeof(t_cmd)); // Correction de l'allocation
				subcount++;
				j++;
			}
			if (split[j][0] == ')')
			{
				subs[current_sub]->cmds[l] = NULL;
				current_sub--;
				j++;
				if (split[j] == NULL)
					break ;
			}
			t_cmd *cmd = malloc(sizeof(t_cmd));
			cmd->cmd = strdup(split[j]);
			cmd->args = malloc(50 * sizeof(char *));
			int k = 0;
			cmd->args[k++] = strdup(split[j++]); // Copier la première partie de la commande
			while (split[j] != NULL && ft_strncmp(split[j], "&&", 2) != 0 && ft_strncmp(split[j], "||", 2) != 0 && ft_strncmp(split[j], ")", 1) != 0) {
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
				} else {
					cmd->args[k++] = strdup(split[j++]); // Copier les arguments suivants
				}
			}
			cmd->previous_element = prev;
			cmd->args[k] = NULL; // Terminer le tableau d'arguments
			subs[current_sub]->cmds[l++] = cmd;
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
		subs[subcount]->cmds[l] = NULL; // Terminer le tableau de commandes
		subs[subcount]->has_heredoc = 0;
		subs[subcount + 1] = NULL;

		print_command_names(subs);

		execute_subshell(subs, envp);

		// Boucle de libération de la mémoire pour chaque commande
		// int i = 0;
		// while (sub1->cmds[i] != NULL) {
		// 	free(sub1->cmds[i]->cmd);
		// 	int p = 0;
		// 	while (sub1->cmds[i]->args[p] != NULL)
		// 		free(sub1->cmds[i]->args[p++]);
		// 	free(sub1->cmds[i]->args);
		// 	free(sub1->cmds[i]);
		// 	i++;
		// }
		// int k = 0;
		// while (split[k] != NULL)
		// 	free(split[k++]);
		// free(split);
		// free(sub1->cmds);
		// free(sub1);
		// free(subs);
	}
	return (0);
}
