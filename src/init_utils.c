#include "minishell.h"

void	init_input(t_input *input)
{
	input->s_quot = 0;
	input->d_quot = 0;
	input->to_expand = 0;
	input->is_open = 0;
}

void	init_cmd(t_command *cmd)
{
	cmd->cmd = NULL;
	cmd->opt = NULL;
	cmd->args = NULL;
	cmd->input_line = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->redir_stdin = 0;
	cmd->redir_stdout = 0;
	cmd->to_pipe = 0;
	cmd->to_pipe_to = 0;
	cmd->first = 0;
	cmd->saved_in = 0;
	cmd->saved_out = 0;
	cmd->input_fd = 0;
	cmd->output_fd = 1;
}

void	take_input(t_input *input)
{
	char	*typed;
	char	*tmp;

	typed = readline("whisper_hole: ");
	check(typed, input);
	if (is_heredoc(typed))
	{
		if (treat_heredoc(typed))
			return ;
	}
	while (input->is_open)
	{
		tmp = readline("> ");
		ft_strlcat(typed, tmp, ft_strlen(typed) + ft_strlen(tmp) + 2);
		free(tmp);
		check(typed, input);
	}
	tmp = ft_strtrim(typed, " ");
	free(typed);
	malloc_and_check_char(&input->line, ft_strlen(tmp) + 1);
	ft_strlcpy(input->line, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

void	take_elem(t_dict *elem, int *ind)
{
	int			i;
	extern char	**environ;

	i = 0;
	if (!elem)
		malloc_and_check_dict(&elem, 1);
	while (environ[*ind][i] != '=')
		i++;
	malloc_and_check_char(&elem->key, i + 1);
	ft_strlcpy(elem->key, environ[*ind], i + 1);
	i++;
	while (environ[*ind][i])
		i++;
	malloc_and_check_char(&elem->value, i - ft_strlen(elem->key));
	ft_strlcpy(elem->value, &environ[*ind][ft_strlen(elem->key) + 1], i - ft_strlen(elem->key));
	elem->next = NULL;
	*ind += 1;
}

void	take_environ(void)
{
	int			i;
	t_dict		*new;
	t_dict		*prev;
	extern char	**environ;

	i = 0;
	new = NULL;
	take_elem(g_term.env, &i);
	prev = g_term.env;
	while (environ[i])
	{
		malloc_and_check_dict(&new, 1);
		prev->next = new;
		prev = new;
		take_elem(new, &i);
	}
}
