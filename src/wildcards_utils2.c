/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 11:15:41 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/10 14:16:08 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sup_get_let(char *brackets, int i, char *ret, char *tmp)
{
	int	len;

	len = 2;
	if (brackets[i] != '-')
	{
		if (i == 1)
			ft_strlcpy(ret, &brackets[i], len++);
		else
			ft_strlcat(ret, &brackets[i], len++);
	}
	else
	{
		tmp = chrs_range(brackets[i - 1], brackets[i + 1]);
		len += ft_strlen(tmp) - 1;
		ft_strlcat(ret, tmp, len++);
		free(tmp);
	}
	return (ret);
}

char	*get_letters(char *brackets)
{
	int		i;
	char	*ret;
	char	*tmp;

	i = 1;
	tmp = NULL;
	ret = malloc(sizeof(char) * count_letters(brackets) + 1);
	while (brackets[i] != ']')
	{
		ret = sup_get_let(brackets, i, ret, tmp);
		i++;
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

int	helper_guess(t_command *cmd)
{
	free_array_of_array(cmd->portions);
	return (-1);
}
