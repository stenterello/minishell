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

void	fill_cmd_fields(char **tmp, t_command *cmd, int start)
{
	int			i;
	int			j;
	int			args_num;
	t_command	*next;

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
	cmd->args[j] = NULL;
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
	{
		i = 0;
		while (tmp[i])
			free(tmp[i++]);
		free(tmp);
	}
	else
		cmd->to_pipe_to = 1;
}

void	split_command(char *line, t_command *cmd)
{
	char	**tmp;

	if (ft_strchr(line, '<') != NULL || ft_strchr(line, '>') != NULL)
		check_redirection(line, cmd);
	if (ft_strchr(line, '|') != NULL)
		check_pipe(line, cmd);
	if (is_var_def(line))
		split_var_decl(line, cmd);
	else
	{
		tmp = split_fields(line, ' ');
		cmd->first = 1;
		fill_cmd_fields(tmp, cmd, 0);
	}
}
