/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:06:53 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/24 19:02:05 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_input(t_input *input)
{
	input->s_quot = 0;
	input->d_quot = 0;
	input->to_expand = 0;
	input->is_open = 0;
	input->with_error = 0;
}

void	init_cmd(t_command *cmd)
{
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->input_line = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->redir_in = 0;
	cmd->redir_out = 0;
	cmd->to_pipe = 0;
	cmd->to_pipe_to = 0;
	cmd->first = 0;
	cmd->saved_in = 0;
	cmd->saved_out = 0;
	cmd->input_fd = 0;
	cmd->output_fd = 1;
	cmd->portions = NULL;
}

void	sup_take_input(char *typed, char *tmp, t_input *input)
{
	tmp = readline("> ");
	if (!tmp)
		unexpected(typed);
	ft_strlcat(typed, tmp, ft_strlen(typed) + ft_strlen(tmp) + 2);
	free(tmp);
	check(typed, input);
}

void	take_input(t_input *input)
{
	char	*typed;
	char	*tmp;

	tmp = NULL;
	typed = readline("whisper_hole: ");
	check(typed, input);
	if (is_heredoc(typed))
	{
		if (treat_heredoc(typed))
			return ;
	}
	while (input->is_open)
		sup_take_input(typed, tmp, input);
	tmp = ft_strtrim(typed, " ");
	free(typed);
	malloc_c(&input->line, ft_strlen(tmp) + 1);
	ft_strlcpy(input->line, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

void	take_elem(t_dict *elem, int *ind)
{
	int			i;
	extern char	**environ;
	char		**tmp;

	i = 0;
	if (!elem)
		malloc_and_check_dict(&elem, 1);
	tmp = environ;
	while (tmp[*ind][i] != '=')
		i++;
	malloc_c(&elem->key, i + 1);
	ft_strlcpy(elem->key, tmp[*ind], i + 1);
	i++;
	while (tmp[*ind][i])
		i++;
	malloc_c(&elem->value, i - ft_strlen(elem->key));
	ft_strlcpy(elem->value, &tmp[*ind][ft_strlen(elem->key) + 1],
		i - ft_strlen(elem->key));
	elem->next = NULL;
	*ind += 1;
}
