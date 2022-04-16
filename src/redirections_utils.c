#include "minishell.h"

void	restore_output(t_command *cmd)
{
	close(STDOUT_FILENO);
	dup2(cmd->saved_out, 1);
	close(cmd->saved_out);
}

void	restore_input(t_command *cmd)
{
	close(STDIN_FILENO);
	dup2(cmd->saved_in, 0);
	close(cmd->saved_in);
}

void	restore_fd(t_command *cmd)
{
	if (cmd->redir_stdout)
		restore_output(cmd);
	if (cmd->redir_stdin)
		restore_input(cmd);
}

void	check_redirection(char *line, t_command *cmd)
{
	if (is_redir(line) == -1)
		return ;
	else if (is_redir(line) == 0)
		define_input(line, cmd);
	else if (is_redir(line) == 1)
		define_output(line, cmd);
	else if (is_redir(line) == 3)
		define_append_output(line, cmd);
}
