#include "minishell.h"

void	execute(t_input *input, t_term *term)
{
	int		i;

	input->args = ft_split(input->line, ' ');
	if (!ft_strncmp(input->args[0], "exit\0", 5))
		exit_cmd(input->args);
	else if (!input->args[1] && ft_strchr(input->args[0], '=') && input->args[0][0] != '=')
		set_sh_var(input->args[0], term);
	else
	{
		if (builtin(input, term))
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
	}
}
