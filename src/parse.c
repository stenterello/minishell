#include "minishell.h"

void	fill_arg(char *line, t_command *cmd, int meas[3])
{
	malloc_and_check_char(&cmd->args[meas[2]], (meas[0] - meas[1] + 1));
	ft_strlcpy(cmd->args[meas[2]], &line[meas[1]], meas[0] - meas[1] + 1);
	cmd->args[meas[2] + 1] = NULL;
}

void	split_var_decl(char *line, t_command *cmd)
{
	int	meas[3];

	meas[0] = 0;
	meas[1] = 0;
	meas[2] = 0;
	malloc_and_check_char_ptr(&cmd->args, count_params(line) + 1);
	cmd->args[0] = NULL;
	while (line[meas[0]] != '=')
		meas[0]++;
	meas[0]++;
	while (line[meas[0]])
	{
		if (line[meas[0]] == ' ' && !is_open(line, meas[0]))
		{
			fill_arg(line, cmd, meas);
			meas[2]++;
			while (line[meas[0]] == ' ')
				meas[0]++;
			meas[1] = meas[0];
		}
		meas[0]++;
	}
	if (!cmd->args[meas[2]])
	{
		malloc_and_check_char(&cmd->args[meas[2]], ft_strlen(line) + 1);
		ft_strlcpy(cmd->args[meas[2]], &line[meas[1]], ft_strlen(&line[meas[1]]) + 1);
		meas[2]++;
	}
	cmd->args[meas[2]] = NULL;
}

void	free_array_of_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	syntax_error(char **tmp)
{
	int		i;
	char	tok;

	ft_putstr_fd(last_field(ft_getenv("SHELL")), 2);
	ft_putstr_fd(": syntax error near unexpected token \"", 2);
	i = 0;
	while (tmp[i])
	{
		if (is_token(tmp[i]))
			tok = tmp[i][0];
		i++;
	}
	ft_putchar_fd(tok, 2);
	ft_putendl_fd("\"", 2);
}

int	fill_cmd_fields(char **tmp, t_command *cmd, int start)
{
	int			i;
	int			j;
	int			args_num;
	t_command	*next;
	t_command	*prev;

	args_num = count_args(tmp);
	malloc_and_check_char_ptr(&cmd->args, args_num + 1);
	i = start;
	j = 0;
	while (tmp[i] && !is_token(tmp[i]))
	{
		if (i == start)
		{
			malloc_and_check_char(&cmd->cmd, next_arg_len(tmp[i]) + 2);
			ft_strlcpy(cmd->cmd, tmp[i], next_arg_len(tmp[i]) + 1);
		}
		if (i != 0 && !ft_strncmp(cmd->cmd, "sed\0", 4))
			tmp[i] = ft_strtrim(tmp[i], "\'");
		malloc_and_check_char(&cmd->args[j], ft_strlen(tmp[i]) + 1);
		ft_strlcpy(cmd->args[j], tmp[i], ft_strlen(tmp[i]) + 1);
		i++;
		j++;
	}
	if (tmp[i])
	{
		if (check_redirection(&tmp[i], cmd) == -1)
		{
			syntax_error(tmp);
			free_array_of_array(tmp);
			return (-1);
		}
	}
	if (cmd->redir_in || cmd->redir_out)
		i += 2;
	if (tmp[i] && tmp[i][0] == '|')
		cmd->to_pipe = 1;
	cmd->args[j] = NULL;
	if (cmd->prev)
	{
		prev = cmd->prev;
		if (prev->to_pipe)
			cmd->to_pipe_to = 1;
	}
	if (tmp[i] && (cmd->to_pipe || cmd->to_pipe_to))
	{
		next = malloc(sizeof(t_command));
		if (!next)
			die("Malloc error");
		init_cmd(next);
		cmd->next = next;
		next->prev = cmd;
		++i;
		fill_cmd_fields(tmp, next, i);
	}
	if (cmd->first)
		free_array_of_array(tmp);
	else
		cmd->to_pipe_to = 1;
	return (0);
}

int	split_command(char *line, t_command *cmd)
{
	char	**tmp;

	if (ft_strchr(line, '|') != NULL)
		check_pipe(line, cmd);
	if (is_var_def(line))
		split_var_decl(line, cmd);
	else
	{
		tmp = split_fields(line, ' ');
		cmd->first = 1;
		if (fill_cmd_fields(tmp, cmd, 0) == -1)
			return (0);
	}
	return (1);
}
