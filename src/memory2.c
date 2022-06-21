/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 18:04:15 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/21 15:03:16 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *cmd)
{
	t_command	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->args)
			free_array_of_array(tmp->args);
		if (tmp->redirections)
			free_array_of_array(tmp->redirections);
		if (tmp->cmd)
			free(tmp->cmd);
		if (tmp->input_line)
			free(tmp->input_line);
		if (!tmp->first)
			free(tmp);
		tmp = tmp->next;
	}
	tmp = cmd;
}

void	free_array_of_array(char **arr)
{
	int	i;

	i = 0;
	if (arr[i])
	{
		while (arr[i])
		{
			free(arr[i]);
			arr[i++] = NULL;
		}
	}
	if (arr)
	{
		free(arr);
		arr = NULL;
	}
}
