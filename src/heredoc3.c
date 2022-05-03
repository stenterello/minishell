/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 16:35:00 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:57:11 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_here(char *delimiter, int i)
{
	ft_putstr_fd(last_field(ft_getenv("SHELL")), 2);
	ft_putstr_fd(": attention: here-document on line ", 2);
	ft_putnbr_fd(i, 2);
	ft_putstr_fd(" is delimited by an EOF (\"", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("\" was required)", 2);
}
