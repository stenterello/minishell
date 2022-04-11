#include "minishell.h"

int	count_params(char *line)
{
	int	count;
	int	i;
	int	s_quot;
	int	d_quot;

	count = 1;
	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (line[i])
	{
		if (line[i] == ' ' && !d_quot && !s_quot)
			count++;
		else if (line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		else if (line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		i++;
	}
	return (count);
}

void	fill_arg(char *line, t_command *cmd, int meas[3])
{
	cmd->args[meas[2]] = malloc(sizeof(char) * (meas[0] - meas[1] + 1));
	if (!cmd->args[meas[2]])
		die("Malloc error");
	ft_strlcpy(cmd->args[meas[2]], &line[meas[1]], meas[0] - meas[1] + 1);
	cmd->args[meas[2] + 1] = NULL;
}

void	split_var_decl(char *line, t_command *cmd)
{
	int	meas[3];

	meas[0] = 0;
	meas[1] = 0;
	meas[2] = 0;
	cmd->args = malloc(sizeof(char *) * (count_params(line) + 1));
	if (!cmd->args)
		die("Malloc error");
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
		cmd->args[meas[2]] = malloc(sizeof(char) * (ft_strlen(line) + 1));
		if (!cmd->args[meas[2]])
			die("Malloc error");
		ft_strlcpy(cmd->args[meas[2]], &line[meas[1]], ft_strlen(&line[meas[1]]) + 1);
		meas[2]++;
	}
	cmd->args[meas[2]] = NULL;
}

int	count_args(char **tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
		i++;
	return (i);
}

void	fill_cmd_fields(char **tmp, t_command *cmd)
{
	int	i;
	int	args_num;

	i = 0;
	args_num = count_args(tmp);
	cmd->args = malloc(sizeof(char *) * (args_num + 1));
	if (!cmd->args)
		die("Malloc error");
	while (tmp[i])
	{
		if (i == 0)
		{
			cmd->cmd = malloc(sizeof(char) * (ft_strlen(tmp[i] + 1)));
			if (!cmd->cmd)
				die("Malloc error");
			ft_strlcpy(cmd->cmd, tmp[i], ft_strlen(tmp[i]) + 1);
		}
		cmd->args[i] = malloc(sizeof(char) * (ft_strlen(tmp[i]) + 1));
		if (!cmd->args[i])
			die("Malloc error");
		ft_strlcpy(cmd->args[i], tmp[i], ft_strlen(tmp[i]) + 1);
		free(tmp[i]);
		i++;
	}
	cmd->args[i] = NULL;
	free(tmp);
}

void	split_command(char *line, t_command *cmd)
{
	char	**tmp;

	if (is_var_def(line))
		split_var_decl(line, cmd);
	else
	{
		tmp = ft_split(line, ' ');
		fill_cmd_fields(tmp, cmd);
	}
}
