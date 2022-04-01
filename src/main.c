#include "minishell.h"

int	go_nl(char *line)
{
	int	i;

	i = 0;
	while (line[i])
		i++;
	if (line[i - 1] == '\\')
	{
		line[i - 1] = 0;
		return (1);
	}
	return (0);
}

char	*read_input(void)
{
	char	*line;
	char	*tmp;

	// Prendi input e salvalo nella history
	line = readline("whisper_hole: ");
	while (go_nl(line))
	{
		tmp = readline("> ");
		ft_strlcat(line, tmp, ft_strlen(line) + ft_strlen(tmp) + 2);
		free(tmp);
	}
	return (line);
}

void	cmd_not_found(char *line)
{
	ft_putstr_fd(line, 2);
	ft_putendl_fd(": command not found", 2);
}

void	elaborate_cmd(char *line)
{
	char	*ret;

	if (!ft_strncmp(line, "pwd\0", 4))
	{
		ret = pwd();
		free(ret);
	}
	else if (!ft_strncmp(line, "exit\0", 5))
		exit(0);
	else if (!ft_strncmp(line, "cd\0", 3))
		cd(line);
	else
		cmd_not_found(line);
}

int	main(int argc, char **argv)
{
	char	*line;
	// char	*var;

	// var = getenv("PATH");
	// printf("%s\n", var);
	while (1)
	{
		line = read_input();
		add_history(line);
		elaborate_cmd(line);
		free(line);
	}
	return (0);
}
