#include "../minishell.h"

int	is_quoted(char	*line)
{
	int	s_quot;
	int	d_quot;
	int	i;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !s_quot && !d_quot)
			return (1);
		else if (line[i] == '\"' && !d_quot && !s_quot)
			return (2);
		i++;
	}
	return (0);
}

void	echo(t_input *input)
{
	int	i;

	i = 1;
	if (!input->args[1])
		ft_putendl_fd(NULL, 1);
	else
	{
		if (!ft_strncmp("-n\0", input->args[1], 3))
			i++;
		while (input->args[i])
		{
			if (is_quoted(input->args[i]) == 2)
				ft_putstr_fd(ft_strtrim(input->args[i++], "\""), 1);
			else if (is_quoted(input->args[i]) == 1)
				ft_putstr_fd(ft_strtrim(input->args[i++], "\'"), 1);
			else
				ft_putstr_fd(input->args[i++], 1);
			if (input->args[i])
				ft_putchar_fd(' ', 1);
		}
		if (ft_strncmp("-n\0", input->args[1], 3))
			ft_putchar_fd('\n', 1);
	}
}