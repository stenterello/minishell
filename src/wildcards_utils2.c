/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 11:15:41 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 16:04:41 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sup_get_let(char *brackets, int *i, char *ret, char *tmp)
{
	if (brackets[i[0]] != '-')
	{
		if (i[0] == 1)
			ft_strlcpy(ret, &brackets[i[0]], i[1]++);
		else
			ft_strlcat(ret, &brackets[i[0]], i[1]++);
	}
	else
	{
		tmp = chrs_range(brackets[i[0] - 1], brackets[i[0] + 1]);
		i[1] += ft_strlen(tmp) - 1;
		ft_strlcat(ret, tmp, i[1]++);
		free(tmp);
	}
	ret[i[1]] = '\0';
	return (ret);
}

char	*get_letters(char *brackets)
{
	int		i[2];
	char	*ret;
	char	*tmp;

	i[0] = 1;
	i[1] = 2;
	tmp = NULL;
	ret = malloc(sizeof(char) * count_letters(brackets) + 1);
	while (brackets[i[0]] != ']')
	{
		ret = sup_get_let(brackets, i, ret, tmp);
		i[0]++;
	}
	return (ret);
}

char	**sup_get_res(struct dirent *en, char **ret, char **port, DIR *stream)
{
	int	i;

	i = 0;
	while (en)
	{
		if (ft_strncmp(en->d_name, ".", 1)
			&& is_verified(en->d_name, port) > 0)
		{
			malloc_c(&ret[i], ft_strlen(en->d_name) + 1);
			ft_strlcpy(ret[i++], en->d_name, ft_strlen(en->d_name) + 1);
		}
		en = readdir(stream);
	}
	ret[i] = NULL;
	closedir(stream);
	return (ret);
}

char	**sup_h_res(struct dirent *en, char **ret, char **port, DIR *stream)
{
	int	i;

	i = 0;
	while (en)
	{
		if (is_verified(en->d_name, port) > 0)
		{
			malloc_c(&ret[i], ft_strlen(en->d_name) + 1);
			ft_strlcpy(ret[i++], en->d_name, ft_strlen(en->d_name) + 1);
		}
		en = readdir(stream);
	}
	ret[i] = NULL;
	closedir(stream);
	return (ret);
}

char	**get_results(char **portions, int len)
{
	DIR				*stream;
	char			**ret;
	struct dirent	*entry;

	malloc_c_ptr(&ret, len + 1);
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (NULL);
	}
	entry = readdir(stream);
	return (sup_get_res(entry, ret, portions, stream));
}
