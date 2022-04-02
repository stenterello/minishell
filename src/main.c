#include "minishell.h"

char	*read_input(void)
{
	char	*line;
	char	*tmp;

	// Prendi input e salvalo nella history
	line = NULL;
	tmp = NULL;
	line = readline("whisper_hole: ");
	while (go_nl(line))
	{
		tmp = readline("> ");
		ft_strlcat(line, tmp, ft_strlen(line) + ft_strlen(tmp) + 2);
		free(tmp);
	}
	tmp = ft_strtrim(line, " ");
	free(line);
	return (tmp);
}

int	main(void)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		line = read_input();
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			tmp = ft_strtrim(line, "\"'");
			free(line);
			line = tmp;
			elaborate_cmd(line);
		}
		//free(line);
	}
	rl_clear_history();
	return (0);
}
