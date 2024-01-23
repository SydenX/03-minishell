/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 11:09:07 by jtollena          #+#    #+#             */
/*   Updated: 2023/10/23 13:25:38 by jtollena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_get_words(const char *s, char c)
{
	int	i;
	int	word;

	if (s[0] == 0)
		return (0);
	word = 0;
	i = 0;
	while (s[i] != 0)
	{
		while (s[i] == c)
			i++;
		if (s[i] != 0)
			word++;
		while (s[i] != c && s[i] != 0)
			i++;
	}
	return (word);
}

static void	*ft_freeall(char **ls)
{
	int	i;

	i = 0;
	while (ls[i] != 0)
	{
		free(ls[i]);
		i++;
	}
	free(ls);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	char	**list;
	int		i;
	int		j;
	int		k;

	if (s == NULL)
		return (NULL);
	list = malloc((ft_get_words(s, c) + 1) * sizeof(char *));
	if (list == NULL)
		return (NULL);
	i = 0;
	k = 0;
	while (k < ft_get_words(s, c))
	{
		while (s[i] == c && s[i] != 0)
			i++;
		j = i;
		while (s[i] != c && s[i] != 0)
			i++;
		list[k] = ft_substr(s, j, i - j);
		if (list[k++] == NULL)
			return (ft_freeall(list));
	}
	list[ft_get_words(s, c)] = 0;
	return (list);
}
