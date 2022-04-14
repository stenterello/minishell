#include "minishell.h"

void	init_input_and_cmd(t_input *input, t_command *cmd)
{
	input->s_quot = 0;
	input->d_quot = 0;
	input->to_expand = 0;
	input->is_open = 0;
	cmd->cmd = NULL;
	cmd->opt = NULL;
	cmd->args = NULL;
	cmd->stdin = STDIN_FILENO;
	cmd->stdout = STDOUT_FILENO;
	cmd->stderr = STDERR_FILENO;
	cmd->next = NULL;
	cmd->redir_stdin = 0;
	cmd->redir_stdout = 0;
	cmd->redir_stderr = 0;
}

void	take_input(t_input *input)
{
	char	*typed;
	char	*tmp;

	typed = readline("whisper_hole: ");
	check(typed, input);
	while (input->is_open)
	{
		tmp = readline("> ");
		ft_strlcat(typed, tmp, ft_strlen(typed) + ft_strlen(tmp) + 2);
		free(tmp);
		check(typed, input);
	}
	tmp = ft_strtrim(typed, " ");
	free(typed);
	input->line = malloc(sizeof(char) * (ft_strlen(tmp) + 1));
	if (!input->line)
		die("Malloc error");
	ft_strlcpy(input->line, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

void	take_elem(t_env_var *elem, int *ind)
{
	int			i;
	extern char	**environ;

	i = 0;
	if (!elem)
	{
		elem = malloc(sizeof(t_env_var));
		if (!elem)
			die("Malloc error");
	}
	while (environ[*ind][i] != '=')
		i++;
	elem->key = malloc(sizeof(char) * (i + 1));
	if (!elem->key)
		die("Malloc error");
	ft_strlcpy(elem->key, environ[*ind], i + 1);
	i++;
	while (environ[*ind][i])
		i++;
	elem->value = malloc(sizeof(char) * (i - ft_strlen(elem->key)));
	if (!elem->value)
		die("Malloc error");
	ft_strlcpy(elem->value, &environ[*ind][ft_strlen(elem->key) + 1], i - ft_strlen(elem->key));
	elem->next = NULL;
	*ind += 1;
}

void	take_environ(void)
{
	int			i;
	t_env_var	*new;
	t_env_var	*prev;
	extern char	**environ;

	i = 0;
	take_elem(g_term.env, &i);
	prev = g_term.env;
	while (environ[i])
	{
		new = malloc(sizeof(t_env_var));
		if (!new)
			die("Malloc error");
		prev->next = new;
		prev = new;
		take_elem(new, &i);
	}
}
