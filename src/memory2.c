/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 18:04:15 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/22 19:18:20 by gimartin         ###   ########.fr       */
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
		if (tmp->redi)
			free_array_of_array(tmp->redi);
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
