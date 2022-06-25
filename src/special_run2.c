/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_run2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:44:11 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/24 20:26:02 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	suspended_cat(t_terminfo *terminfo)
{
	int	i;

	i = 0;
	while (i < terminfo->suspended_cat)
	{
		terminfo->input->line = readline("");
		if (!ft_strlen(terminfo->input->line))
			ft_putchar_fd('\n', STDOUT_FILENO);
		free(terminfo->input->line);
		i++;
	}
}
