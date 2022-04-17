#include "minishell.h"

void	print_error(t_command *cmd)
{
	ft_putstr_fd(cmd->cmd, STDERR_FILENO);
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

void	kill_proc(int sig, siginfo_t *info, void *context)
{
	(void) info;
	(void) context;
	if (sig == SIGINT)
	{
		printf("ao\n");
		kill(0, SIGINT);
	}
}

void	child_signals(void)
{
	int					sigs[3];
	struct sigaction	acts;

	acts.sa_sigaction = &kill_proc;
	sigemptyset(&acts.sa_mask);
	sigs[0] = sigaction(SIGINT, &acts, NULL);
	if (sigs[0])
		die("Signal error");
	printf("AO");
}

void	execute(t_command *cmd)
{
	int			i;
	int			ret;
	int			child;
	t_command	*tmp;

	i = 0;
	ret = 0;
	tmp = cmd;
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
				child = fork();
				if (child == -1)
					die("Error while forking");
				if (child == 0)
				{
					free_dict(g_term.var);
					// crea segnali per il processo figlio
					// child_signals();
					execve(tmp->cmd, tmp->args, NULL);
				}
				else
					waitpid(child, &ret, 0);
				if (WIFEXITED(ret))
					g_term.last_exit = ret / 256;
				else
					g_term.last_exit = ret;
			}
			i = 0;
			while (tmp->args[i])
				free(tmp->args[i++]);
			free(tmp->args);
			free(tmp->cmd);
		}
		tmp = tmp->next;
	}
}
