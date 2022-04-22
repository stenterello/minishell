#include "minishell.h"

void	restore_output(t_command *cmd)
{
	close(STDOUT_FILENO);
	dup2(cmd->saved_out, STDOUT_FILENO);
	close(cmd->saved_out);
}

void	restore_input(t_command *cmd)
{
	close(STDIN_FILENO);
	dup2(cmd->saved_in, STDIN_FILENO);
	close(cmd->saved_in);
}

void	restore_fd(t_command *cmd)
{
	if (cmd->redir_stdout || cmd->to_pipe)
		restore_output(cmd);
	if (cmd->redir_stdin || cmd->to_pipe_to)
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

void	check_pipe(char *line, t_command *cmd)
{
	int			i;
	int			s_quot;
	int			d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		else if (line[i] == '"' && !d_quot && !s_quot)
			d_quot = 1;
		else if (line[i] == '"' && d_quot && !s_quot)
			d_quot = 0;
		else if (line[i] == '|' && !d_quot && !s_quot && line[i + 1] != '|')
			cmd->to_pipe = 1;
		i++;
	}
}

void	define_pipe(t_command *cmd)
{
	int			piped[2];
	t_command	*tmp;

	if (pipe(piped) == -1)
		die("Error while piping");
	cmd->piped_fd = piped[1];
	tmp = cmd->next;
	tmp->piped_fd = piped[0];
	cmd->saved_out = dup(1);
	close(1);
	dup2(cmd->piped_fd, 1);
	close(cmd->piped_fd);
}

void	define_pipe_to(t_command *cmd)
{
	cmd->saved_in = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	dup2(cmd->piped_fd, STDIN_FILENO);
}

void	define_heredoc_pipe(t_command *cmd)
{
	int			piped[2];
	t_command	*tmp;

	if (pipe(piped) == -1)
		die("Error while piping");
	cmd->piped_fd = piped[1];
	tmp = cmd->next;
	tmp->piped_fd = piped[0];
	tmp->saved_in = dup(STDOUT_FILENO);
	close(STDIN_FILENO);
	dup2(tmp->piped_fd, STDIN_FILENO);
	close(tmp->piped_fd);
}
