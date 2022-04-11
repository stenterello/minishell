#include "minishell.h"

void	rewrite_args(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (is_var_def(cmd->args[i]))
		i++;
	cmd->cmd = malloc(sizeof(char) * (ft_strlen(cmd->args[i]) + 1));
	if (!cmd->cmd)
		die("Malloc error");
	ft_strlcpy(cmd->cmd, cmd->args[i], ft_strlen(cmd->args[i]) + 1);
	while (cmd->args[i])
	{
		free(cmd->args[j]);
		cmd->args[j] = malloc(sizeof(char) * (ft_strlen(cmd->args[i]) + 1));
		if (!cmd->args[j])
			die("Malloc error");
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

void	execute(t_command *cmd, t_term *term)
{
	int		i;

	i = 0;
	if (!cmd->cmd)
	{
		while (cmd->args[i])
		{
			if (!is_var_def(cmd->args[i]))
			{
				rewrite_args(cmd);
				execute(cmd, term);
				return ;
			}
			i++;
		}
		set_sh_var(cmd->args, term);
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		free(cmd->cmd);
		return ;
	}
	else if (!cmd->cmd && ft_strchr(cmd->args[0], '=') && cmd->args[0][0] != '=' && cmd->args[1])
		rewrite_args(cmd);
	else if (!ft_strncmp(cmd->cmd, "exit\0", 5))
	{
		exit_cmd(cmd->args);
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		free(cmd->cmd);
		return ;
	}	
	if (!builtin(cmd, term))
	{
		if (ft_strchr(cmd->cmd, '/') == NULL)
		{
			if (find_script(cmd) == -1)
				cmd_not_found(cmd->cmd);
		}
		else
		{
			if (execve(cmd->cmd, cmd->args, NULL) == -1)
				cmd_not_found(cmd->cmd);
		}
	}
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free(cmd->cmd);
}
