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

void	execute_tree(t_command *cmd)
{
	int			i;
	int			ret;
	t_command	*tmp;
	t_command	*tmp2;

	i = 0;
	ret = 0;
	tmp = cmd;
	if (!tmp->cmd && !g_term.delimiter)
	{
		treat_var_decl(tmp);
		return ;
	}
	else if (!tmp->cmd && ft_strchr(tmp->args[0], '=') && tmp->args[0][0] != '=' && tmp->args[1])
		rewrite_args(tmp);
	else if (!ft_strncmp(tmp->cmd, "exit\0", 5))
	{
		exit_cmd(tmp);
		return ;
	}
	else if (tmp->input_line)
	{
		ft_putstr_fd(tmp->input_line, tmp->piped_fd);
		close(tmp->piped_fd);
		waitpid(g_term.child, &ret, 0);
		if (WIFEXITED(ret))
			g_term.last_exit = ret / 256;
		else
			g_term.last_exit = ret;
		g_term.child = 0;
		tmp2 = cmd->prev;
		restore_fd(tmp);
		restore_fd(tmp2);
		free(tmp->input_line);
		free(tmp);
	}
	while (tmp)
	{
		if (!builtin(tmp))
		{
			// Se non c'è indirizzo del file, cerca nel PATH
			if (ft_strchr(tmp->cmd, '/') == NULL)
			{
				if (find_script(tmp) == -1)
					cmd_not_found(tmp);
			}
			else
			{
				if (is_directory(tmp))
					return ;
				g_term.child = fork();
				if (g_term.child == -1)
					die("Error while forking");
				if (g_term.child == 0)
				{
					free_dict(g_term.var);
					execve(tmp->cmd, tmp->args, NULL);
				}
				else if (!g_term.delimiter)
				{
					waitpid(g_term.child, &ret, 0);
					if (WIFEXITED(ret))
						g_term.last_exit = ret / 256;
					else
						g_term.last_exit = ret;
					g_term.child = 0;
				}
			}
		}
		tmp = tmp->next;
	}
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
