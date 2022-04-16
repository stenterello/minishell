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

void	fill_cmd_fields(char **tmp, t_command *cmd)
{
	int	i;
	int	args_num;

	i = 0;
	args_num = count_args(tmp);
	malloc_and_check_char_ptr(&cmd->args, args_num + 1);
	// Occhio al rischio di segfault nei controlli condizionali qui sotto
	while (tmp[i] && ft_strncmp(tmp[i], "<\0", 2) && ft_strncmp(tmp[i], "<<\0", 3) && ft_strncmp(tmp[i], ">\0", 2) && ft_strncmp(tmp[i], ">>\0", 3))
	{
		if (i == 0)
		{
			malloc_and_check_char(&cmd->cmd, next_arg_len(tmp[i]) + 1);
			ft_strlcpy(cmd->cmd, tmp[i], next_arg_len(tmp[i]) + 1);
		}
		malloc_and_check_char(&cmd->args[i], next_arg_len(tmp[i]) + 1);
		ft_strlcpy(cmd->args[i], tmp[i], next_arg_len(tmp[i]) + 1);
		free(tmp[i]);
		i++;
	}
	cmd->args[i] = NULL;
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
}

void	split_command(char *line, t_command *cmd)
{
	char	**tmp;

	if (ft_strchr(line, '<') != NULL || ft_strchr(line, '>') != NULL)
		check_redirection(line, cmd);
	if (is_var_def(line))
		split_var_decl(line, cmd);
	else
	{
		tmp = ft_split(line, ' ');
		fill_cmd_fields(tmp, cmd);
	}
}

// void	split_command(char *line, t_command *cmd)
// {
// 	if (ft_strchr(line, '<') != NULL || ft_strchr(line, '>') != NULL)
// 		check_redirection(line, cmd);
// 	if (is_var_def(line))
// 		split_var_decl(line, cmd);
// 	else
// 	{
// 		malloc_and_check_list(&cmd->args_list, 1);
// 		expression_tree(line, cmd->args_list);
// 		// tmp = ft_split(line, ' ');
// 		// fill_cmd_fields(tmp, cmd);
// 	}
// }
