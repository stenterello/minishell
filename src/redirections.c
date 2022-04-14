#include "minishell.h"

int	file_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_isalnum(line[i]))
		i++;
	return (i);
}

void	define_input(char *line, t_command *cmd)
{
	int		i;
	int		len;
	int		fd;
	char	*input;
	
	i = 0;
	while (line[i] != '<')
		i++;
	i++;
	while (!ft_isalnum(line[i]))
		i++;
	len = file_len(&line[i]);
	input = malloc(sizeof(char) * (len + 1));
	if (!input)
		die("Malloc error");
	ft_strlcpy(input, &line[i], len + 1);
	fd = open(input, O_CREAT);
	if (fd < 0)
		die("Error while opening file");
	dup2(fd, 0);
	cmd->stdin = 0;
}

void	define_output(char *line, t_command *cmd)
{
	int		i;
	int		len;
	int		fd;
	char	*file;
	
	i = 0;
	while (line[i] != '>')
		i++;
	i++;
	while (!ft_isalnum(line[i]))
		i++;
	len = file_len(&line[i]);
	file = malloc(sizeof(char) * (len + 1));
	if (!file)
		die("Malloc error");
	ft_strlcpy(file, &line[i], len + 1);
	fd = open(file, O_WRONLY | O_CREAT, 0666);
	if (fd < 0)
		die("Error while opening file");
	dup2(fd, STDOUT_FILENO);
	cmd->stdin = fd;
}
