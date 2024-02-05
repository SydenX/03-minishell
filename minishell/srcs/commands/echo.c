/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 12:11:25 by jtollena          #+#    #+#             */
/*   Updated: 2024/02/05 10:43:21 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	special_print(char *str, int ln)
{
	int	i;

	i = 0;
	while (i < ln)
	{
		if (str[i] != 0)
			ft_putchar_fd(str[i], 1);
		i++;
	}
}

void	print_env(char *env, char *out)
{
	int	i;

	i = 0;
	while (env[i] == out[i + 1])
		i++;
	printf("%s", &env[i + 1]);
}

int	get_env(char *out, char **env)
{
	int		i;
	char	*location;

	i = 0;
	while (env[i] != 0)
	{
		location = ft_strnstr(env[i], &out[1], ft_strlen(env[i]));
		if (location != 0)
		{
			if (out[0] == '$')
			{
				print_env(env[i], out);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	check_for_env(char *out, char **env)
{
	int	i;

	i = 0;
	while (out[i] != 0)
	{
		if (out[i] == '$')
			return (get_env(&out[i], env));
		i++;
	}
	return (0);
}

void	print_echo(char *out, char **env, int has_param)
{
	int		i;
	int		newout;

	i = 0;
	if (out[0] == '\'' && out[ft_strlen(out) - 1] == '\'')
	{
		while (++i < ft_strlen(out) - 1)
			printf("%c", out[i]);
	}
	else
	{
		// newout = replace(&out, '"', 0);
		if (get_env(out, env) == 0)
		{
			if (ft_strncmp(out, "$?", 2) == 0)
				printf("%d", exit_code);
			else
				special_print(out, ft_strlen(out));
		}
	}
	if (has_param == 0)
		printf("\n");
}

void	echo(char **out, int has_param, char **env)
{
	int	i;

	i = 0;
	while (out[i] != 0)
		print_echo(out[i++], env, has_param);
	exit_code = 0;
}
