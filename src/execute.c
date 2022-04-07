#include "minishell.h"

void	execute(t_input *input)
{
	int		i;
	// pid_t	child;
	// int		status;
	
	input->args = ft_split(input->line, ' ');
	if (!ft_strncmp(input->args[0], "exit\0", 5))
		exit_cmd(input->args);
	else
	{
		// child = fork();
		// if (child == -1)
		// 	die("Error while foriking");
		// if (child == 0)
		// {
			if (builtin(input))
			{
				i = 0;
				while (input->args[i])
					free(input->args[i++]);
				free(input->args);
			}
			else
			{
				if (ft_strchr(input->args[0], '/') == NULL)
				{
					if (find_script(input->args) == -1)
						cmd_not_found(input->line);
				}
				else
				{
					if (execve(input->args[0], input->args, NULL) == -1)
						cmd_not_found(input->line);
				}
				i = 0;
				while (input->args[i])
					free(input->args[i++]);
				free(input->args);
			}
			// exit(0);
		// }
		// else
		// 	waitpid(child, &status, 0);
		// if (WIFEXITED(status))
		// 	g_term->last_exit = WEXITSTATUS(status);
	}
}
