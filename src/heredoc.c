#include "minishell.h"

int	delimiter_len(char *line)
{
	int	i;

	i = 0;
	while (ft_isalnum(line[i]) || is_open(line, i))
		i++;
	return (i);
}

char	*take_delimiter(char *line)
{
	int		i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (ft_strncmp(&line[i], " <<", 3))
		i++;
	i += 4;
	while (!ft_isalnum(line[i]))
		i++;
	malloc_and_check_char(&ret, delimiter_len(&line[i]) + 1);
	ft_strlcpy(ret, &line[i], delimiter_len(&line[i]) + 1);
	return (ret);
}

int	to_continue(char *typed, char *delimiter)
{
	if (ft_strlen(typed) == ft_strlen(delimiter) && !ft_strncmp(typed, delimiter, ft_strlen(delimiter)))
		return (0);
	return (1);
}

void	clean_heredoc(char *line, char *bench)
{
	int	i;

	i = 0;
	while (ft_strncmp(&line[i], bench, ft_strlen(bench)) && i <= (int)(ft_strlen(line) - (int)ft_strlen(bench)))
		i++;
	while (!ft_isalnum(line[i]))
		i--;
	line[++i] = '\0';
}

char	*to_string(char *line)
{
	int		i;
	char	*ret;

	ret = NULL;
	malloc_and_check_char(&ret, ft_strlen(line) + 3);
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

int	treat_heredoc(char *typed)
{
	t_command	*cmd;
	t_command	*cmd2;
	char		*delimiter;
	char		*tmp;
	char		*tmp2;
	
	g_term.delimiter = 1;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		die("Malloc error");
	init_cmd(cmd);
	cmd2 = malloc(sizeof(t_command));
	if (!cmd2)
		die("Malloc error");
	init_cmd(cmd2);
	malloc_and_check_char(&cmd->cmd, ft_strlen(typed) + 1);
	ft_strlcpy(cmd->cmd, typed, ft_strlen(typed) + 1);
	cmd->input_line = NULL;
	cmd2->input_line = NULL;
	delimiter = take_delimiter(cmd->cmd);
	clean_heredoc(cmd->cmd, "<<");
	malloc_and_check_char_ptr(&cmd->args, 2);
	malloc_and_check_char(&cmd->args[0], ft_strlen(cmd->cmd) + 1);
	ft_strlcpy(cmd->args[0], cmd->cmd, ft_strlen(cmd->cmd) + 1);
	cmd->args[1] = NULL;
	tmp = readline("> ");
	while (tmp && ft_strncmp(tmp, delimiter, ft_strlen(delimiter) + 1))
	{
		if (cmd2->input_line)
		{
			malloc_and_check_char(&tmp2, ft_strlen(cmd2->input_line) + 1);
			ft_strlcpy(tmp2, cmd2->input_line, ft_strlen(cmd2->input_line) + 1);
			free(cmd2->input_line);
			malloc_and_check_char(&cmd2->input_line, ft_strlen(tmp2) + ft_strlen(tmp) + 3);
			ft_strlcpy(cmd2->input_line, tmp2, ft_strlen(tmp2) + 1);
			ft_strlcat(cmd2->input_line, "\n", ft_strlen(cmd2->input_line) + 2);
			ft_strlcat(cmd2->input_line, tmp, ft_strlen(tmp) + ft_strlen(tmp2) + 2);
			ft_strlcat(cmd2->input_line, "\n", ft_strlen(cmd2->input_line) + 2);
			free(tmp2);
		}
		else
		{
			malloc_and_check_char(&cmd2->input_line, ft_strlen(tmp) + 1);
			ft_strlcpy(cmd2->input_line, tmp, ft_strlen(tmp) + 1);
		}
		free(tmp);
		tmp = readline("> ");
	}
	free(tmp);
	free(delimiter);
	cmd->next = cmd2;
	cmd2->prev = cmd;
	define_pipe(cmd);
	define_pipe_to(cmd2);
	execute_tree(cmd);
	free(cmd);
	free(cmd2);
	return (1);
}
