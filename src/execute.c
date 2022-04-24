#include "minishell.h"

void	rewrite_args(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (is_var_def(cmd->args[i]))
		i++;
	malloc_and_check_char(&cmd->cmd, ft_strlen(cmd->args[i]) + 1);
	ft_strlcpy(cmd->cmd, cmd->args[i], ft_strlen(cmd->args[i]) + 1);
	while (cmd->args[i])
	{
		free(cmd->args[j]);
		malloc_and_check_char(&cmd->args[j], ft_strlen(cmd->args[i]) + 1);
		ft_strlcpy(cmd->args[j], cmd->args[i], ft_strlen(cmd->args[i]) + 1);
		free(cmd->args[i]);
		cmd->args[i] = NULL;
		i++;
		j++;
	}
	while (cmd->args[j])
	{
		free(cmd->args[j]);
		cmd->args[j++] = NULL;
	}
}

void	born_child(t_command *tmp)
{
	int	status;

	if (tmp->to_pipe)
		define_pipe(tmp);
	if (tmp->to_pipe_to)
		define_pipe_to(tmp);
	g_term.child = fork();
	if (g_term.child == -1)
		die("Error while forking");
	if (g_term.child == 0)
	{
		free_dict(g_term.var);
		execve(tmp->cmd, tmp->args, NULL);
	}
	else
	{
		if (!g_term.delimiter)
		{
			waitpid(g_term.child, &status, 0);
			if (WIFEXITED(status))
				g_term.last_exit = status / 256;
			else
				g_term.last_exit = status;
			g_term.child = 0;
			restore_fd(tmp);
		}
		else
		{
			close(STDIN_FILENO);
			close(tmp->output_fd);
			waitpid(g_term.child, &status, 0);			
			if (WIFEXITED(status))
				g_term.last_exit = status / 256;
			else
				g_term.last_exit = status;
			g_term.child = 0;
			dup2(tmp->saved_in, STDIN_FILENO);
			close(tmp->saved_in);
		}
	}
}

int	preliminary(t_command *tmp)
{
	if (!tmp->cmd && !g_term.delimiter)
	{
		treat_var_decl(tmp);
		return (1);
	}
	else if (!tmp->cmd && tmp->args && ft_strchr(tmp->args[0], '=') && tmp->args[0][0] != '=' && tmp->args[1])
		rewrite_args(tmp);
	return (0);
}

void	free_commands(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i = 0;
		if (tmp->args)
		{
			while (tmp->args[i])
				free(tmp->args[i++]);
			free(tmp->args);
		}
		if (tmp->cmd)
			free(tmp->cmd);
		if (tmp->input_line)
			free(tmp->input_line);
		tmp = tmp->next;
	}
}

void	execute_tree(t_command *cmd)
{
	t_command	*tmp;

	tmp = cmd;
	if (preliminary(tmp))
		return ;
	while (tmp)
	{
		if (!builtin(tmp))
		{
			if (tmp->input_line)
			{
				ft_putstr_fd(tmp->input_line, tmp->output_fd);
				close(tmp->output_fd);
			}
			else if (ft_strchr(tmp->cmd, '/') == NULL)
			{
				if (find_script(tmp) == -1)
					cmd_not_found(tmp);
			}
			else
			{
				if (is_directory(tmp))
					return ;
				born_child(tmp);
			}
		}
		tmp = tmp->next;
	}
	free_commands(cmd);
}
