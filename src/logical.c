#include "minishell.h"

int	is_logical(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_strlen(line) - i > 1)
	{
		if (!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2))
			return (1);
		i++;
	}
	return (0);
}

int	count_cmds(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 1;
	while (line[i] && ft_strlen(line) - i > 1)
	{
		if (!ft_strncmp(&line[i], "&&", 2) || ft_strncmp(&line[i], "||", 2))
		{
			i += 2;
			ret++;
		}
		else
			i++;
	}
	return (ret);
}

int	cmd_len(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2))
			return (i);
		i++;
	}
	return (0);
}

void	define_cmds_logic(char *line, t_command *cmd, char **cmd_lines)
{
	
}

void	get_logical(char *line, t_command *cmd)
{
	int		i;
	int		j;
	char	**cmd_lines;

	i = 0;
	j = 0;
	malloc_and_check_char_ptr(&cmd_lines, count_cmds(line) + 1);
	while (i < count_cmds(line))
	{
		malloc_and_check_char(&cmd_lines[i], cmd_len(&line[j]) + 1);
		ft_strlcpy(cmd_lines[i], &line[j], cmd_len(&line[j]) + 1);
		i++;
		j += cmd_len(&line[j]);
	}
	define_cmds_logic(line, cmd, cmd_lines);
	free_array_of_array(cmd_lines);
}
