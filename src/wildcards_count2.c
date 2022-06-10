/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_count2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:05:43 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 16:07:22 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_h_results(char **portions)
{
	DIR				*stream;
	int				ret;
	struct dirent	*entry;

	ret = 0;
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	entry = readdir(stream);
	while (entry)
	{
		if (is_verified(entry->d_name, portions) > 0)
			ret++;
		else if (is_verified(entry->d_name, portions) == -1)
			return (-1);
		entry = readdir(stream);
	}
	closedir(stream);
	return (ret);
}
