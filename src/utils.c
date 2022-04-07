#include "minishell.h"

void	die(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(-1);
}

int	ft_isupper(char c)
{
	if (c >= 65 && c <= 90)
		return (1);
	return (0);
}

int	var_name_len(char *variable)
{
	int	i;

	i = 0;
	while (ft_isupper(variable[i]))
		i++;
	return (i);
}

void	take_variable(char *variable, t_input *input, int init_len)
{
	int		i;
	char	*var_name;
	char	*ret;

	i = -1;
	var_name = malloc(sizeof(char) * (var_name_len(variable) + 1));
	if (!var_name)
		die("Malloc error");
	while (ft_isupper(variable[++i]))
		var_name[i] = variable[i];
	var_name[i] = '\0';
	input->expanded = getenv(var_name);
	if (input->expanded == NULL)
		input->expanded = ft_calloc(1, sizeof(char));
	free(var_name);
	ret = malloc(sizeof(char) * (ft_strlen(input->line) + 1 - i + ft_strlen(input->expanded)));
	if (!ret)
		die("Malloc error");
	ft_strlcpy(ret, input->line, init_len + 1);
	ft_strlcpy(&ret[init_len], input->expanded, ft_strlen(input->expanded) + 1);
	if (input->line[var_name_len(variable) + 1 + init_len])
		ft_strlcpy(&ret[init_len + ft_strlen(input->expanded)], &input->line[init_len + var_name_len(variable) + 1], ft_strlen(input->line) - (init_len + var_name_len(variable)) + 1);
	free(input->line);
	input->line = ret;
}

void	try_expand(t_input *input)
{
	int	i;

	i = 0;
	while (input->line[i])
	{
		if (input->line[i] == '$')
		{
			i++;
			if (ft_isupper(input->line[i]) || input->line[i] == '?')
				take_variable(&input->line[i], input, i - 1);
			else
				return ;
		}
		i++;
	}
}

int	builtin(t_input *input)
{
	char	*ret;

	input->args = ft_split(input->line, ' ');
	if (!ft_strncmp(input->args[0], "pwd\0", 4))
	{
		ret = pwd();
		ft_putendl_fd(ret, 1);
		free(ret);
	}
	else if (!ft_strncmp(input->args[0], "exit\0", 5))
		exit_cmd(input->args);
	else if (!ft_strncmp(input->args[0], "cd\0", 3))
		cd(input->args);
	else if (!ft_strncmp(input->args[0], "echo\0", 5))
		echo(input);
	else
		return (0);
	return (1);
}



char	*get_path(char *line)
{
	char	*ret;
	int		i;

	i = 0;
	while (line[i] == 32 || (line[i] > 8 && line[i] < 14))
		i++;
	ret = malloc(sizeof(char) * (ft_strlen(line) - i + 1));
	if (!ret)
		die("Malloc error trying to get absolute path [CD]");
	ft_strlcpy(ret, &line[i], ft_strlen(line) - i + 1);
	return (ret);
}
