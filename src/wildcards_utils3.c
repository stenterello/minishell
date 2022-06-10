/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:00:35 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 16:04:46 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_hidden_results(char **portions, int len)
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
	return (sup_h_res(entry, ret, portions, stream));
}

int	helper_guess(t_command *cmd)
{
	free_array_of_array(cmd->portions);
	return (-1);
}
