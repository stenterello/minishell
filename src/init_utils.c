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
	cmd->next = NULL;
	cmd->redir_stdin = 0;
	cmd->redir_stdout = 0;
	cmd->redir_stderr = 0;
}

int	delimiter_len(char *line)
{
	int	i;

	i = 0;
	while (ft_isalnum(line[i]))
		i++;
	return (i);
}

char	*take_delimiter(char *line)
{
	int		i;
	char	*ret;

	i = 0;
	while (ft_strncmp(&line[i], " <<", 3))
		i++;
	i += 4;
	while (!ft_isalnum(line[i]))
		i++;
	ret = malloc(sizeof(char) * (delimiter_len(&line[i]) + 1));
	if (!ret)
		die("Malloc error");
	ft_strlcpy(ret, &line[i], delimiter_len(&line[i]) + 1);
	return (ret);
}

int	to_continue(char *typed, char *delimiter)
{
	int	i;

	i = ft_strlen(typed) - ft_strlen(delimiter);
	if (!ft_strncmp(&typed[i], delimiter, ft_strlen(delimiter) + 1))
		return (0);
	return (1);
}

void	clean_heredoc(char *line, char *bench)
{
	int	i;

	i = 0;
	while (ft_strncmp(&line[i], bench, ft_strlen(bench)) && i <= (int)(ft_strlen(line) - ft_strlen(bench)))
		i++;
	//line[i] = '\"';
	line[i] = '\0';
}

char	*to_string(char *line)
{
	int		i;
	char	*ret;

	ret = malloc(sizeof(char) * (ft_strlen(line) + 3));
	if (!ret)
		die("Malloc error");
	i = 0;
	while (line[i] != ' ')
		i++;
	i++;
	ft_strlcpy(ret, line, i + 1);
	ft_strlcat(ret, "\"", ft_strlen(ret) + 2);
	ft_strlcat(ret, &line[i + 1], ft_strlen(&line[i]) + ft_strlen(ret) + 1);
	ft_strlcat(ret, "\"", ft_strlen(ret) + 2);
	return (ret);
}

void	take_input(t_input *input)
{
	char	*typed;
	char	*tmp;
	char	*delimiter;

	typed = readline("whisper_hole: ");
	check(typed, input);
	if (is_heredoc(typed))
	{
		delimiter = take_delimiter(typed);
		clean_heredoc(typed, "<<");
		tmp = readline("> ");
		ft_strlcat(typed, tmp, ft_strlen(typed) + ft_strlen(tmp) + 2);
		free(tmp);
		while (to_continue(typed, delimiter))
		{
			tmp = readline("> ");
			ft_strlcat(typed, tmp, ft_strlen(typed) + ft_strlen(tmp) + 2);
			free(tmp);
		}
		clean_heredoc(typed, delimiter);
		tmp = to_string(typed);
		free(typed);
		typed = tmp;
		//ft_strlcat(typed, "\"", ft_strlen(typed) + 1 + 2);
		free(delimiter);
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
