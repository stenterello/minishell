#include "minishell.h"

void	define_input(char *line, t_command *cmd)
{
	cmd->input_fd = open(line, O_CREAT, 0666);
	if (cmd->input_fd < 0)
		die("Error while opening file");
	cmd->saved_in = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	dup2(cmd->input_fd, STDIN_FILENO);
	cmd->redir_in = 1;
}

void	define_output(char *line, t_command *cmd)
{
	cmd->output_fd = open(line, O_WRONLY | O_CREAT, 0666);
	if (cmd->output_fd < 0)
		die("Error while opening file");
	cmd->saved_out = dup(1);
	close(1);
	dup2(cmd->output_fd, STDOUT_FILENO);
	cmd->redir_out = 1;
}

void	define_append_output(char *line, t_command *cmd)
{
	cmd->output_fd = open(line, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (cmd->output_fd < 0)
		die("Error while opening file");
	cmd->saved_out = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	dup2(cmd->output_fd, STDOUT_FILENO);
	cmd->redir_out = 1;
}
