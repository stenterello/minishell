#include "minishell.h"

void	print_error(t_command *cmd)
{
	ft_putstr_fd(cmd->cmd, cmd->stderr);
	strerror(errno);
	g_term.last_exit = errno;
}

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

void	child_signals(void)
{
	
}

void	execute(t_command *cmd)
{
	int	i;
	int	ret;
	int	child;

	i = 0;
	if (!cmd->cmd)
	{
		while (cmd->args[i])
		{
			if (!is_var_def(cmd->args[i]))
			{
				rewrite_args(cmd);
				execute(cmd);
				return ;
			}
			i++;
		}
		set_sh_var(cmd->args);
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
		exit_cmd(cmd);
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		free(cmd->cmd);
		return ;
	}	
	if (!builtin(cmd))
	{
		// Se non c'è indirizzo del file, cerca nel PATH
		if (ft_strchr(cmd->cmd, '/') == NULL)
		{
			if (find_script(cmd) == -1)
				cmd_not_found(cmd);
			close(cmd->stdin);
		}
		else
		{
			// Altrimenti vai alla destinazione
			child = fork();
			if (child == -1)
				die("Error while forking");
			if (child == 0)
			{
				// cancella variabili di shell
				// crea segnali per il processo figlio
				// // creare una funzione born_child?
				//child_signals();
				execve(cmd->cmd, cmd->args, NULL);
			}
			else
				waitpid(child, &ret, 0);
			if (WIFEXITED(ret))
				g_term.last_exit = ret / 256;
			else
				g_term.last_exit = ret;
			close(cmd->stdin);
		}
	}
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free(cmd->cmd);
}
