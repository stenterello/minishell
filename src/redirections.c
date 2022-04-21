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
	char	*input;
	
	i = 0;
	input = NULL;
	while (line[i] != '<')
		i++;
	i++;
	while (!ft_isalnum(line[i]))
		i++;
	len = file_len(&line[i]);
	malloc_and_check_char(&input, len + 1);
	ft_strlcpy(input, &line[i], len + 1);
	cmd->fd = open(input, O_CREAT, 0666);
	if (cmd->fd < 0)
		die("Error while opening file");
	cmd->saved_in = dup(0);
	close(0);
	dup2(cmd->fd, 0);
	free(input);
	cmd->redir_stdin = 1;
}

void	define_output(char *line, t_command *cmd)
{
	int		i;
	int		len;
	char	*file;
	
	i = 0;
	file = NULL;
	while (line[i] != '>')
		i++;
	i++;
	while (!ft_isalnum(line[i]))
		i++;
	len = file_len(&line[i]);
	malloc_and_check_char(&file, len + 1);
	ft_strlcpy(file, &line[i], len + 1);
	cmd->fd = open(file, O_WRONLY | O_CREAT, 0666);
	if (cmd->fd < 0)
		die("Error while opening file");
	cmd->saved_out = dup(1);
	close(1);
	dup2(cmd->fd, STDOUT_FILENO);
	free(file);
	cmd->redir_stdout = 1;
}

void	define_append_output(char *line, t_command *cmd)
{
	int		i;
	int		len;
	char	*file;
	
	i = 0;
	file = NULL;
	while (line[i] != '>')
		i++;
	i++;
	while (!ft_isalnum(line[i]))
		i++;
	len = file_len(&line[i]);
	malloc_and_check_char(&file, len + 1);
	ft_strlcpy(file, &line[i], len + 1);
	cmd->fd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (cmd->fd < 0)
		die("Error while opening file");
	free(file);
	cmd->saved_out = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	dup2(cmd->fd, STDOUT_FILENO);
	cmd->redir_stdout = 1;
}
