#include "minishell.h"

void	copy_in(char *line, char *splitted)
{
	line = malloc(sizeof(char) * (ft_strlen(splitted) + 1));
	if (!line)
		die("Malloc error");
	ft_strlcpy(line, splitted, ft_strlen(splitted) + 1);
}

void	copy_in_arg(char **args, char *line)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	args[i] = malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!args[i])
		die("Malloc error");
	copy_in(args[i], line);
	args[++i] = NULL;
}

void	alloc_args(t_command *parsed, char **splitted)
{
	int	i;

	i = 0;
	while (splitted[i])
		i++;
	parsed->args = malloc(sizeof(char *) * i);
	if (!parsed->args)
		die ("Malloc error");
	parsed->args[0] = NULL;
}

void	parse(t_input *input, t_command *parsed)
{
	char	**splitted;
	char	*tokens;
	int		i;
	int		i2;
	int		j;

	splitted = ft_split(input->line, ' ');
	tokens = malloc(sizeof(char) * 4);
	if (!tokens)
		die("Malloc error");
	ft_strlcpy(tokens, "<>|", 4);
	i = 0;
	alloc_args(parsed, splitted);
	while (splitted[i])
	{
		if (ft_strchr(tokens, splitted[i][0]) != NULL)
			break;
		if (i == 1)
			copy_in(parsed->cmd, splitted[i]);
		else if (i > 1 && is_option(splitted))
			copy_in(parsed->opt, splitted[i]);
		else
			copy_in_arg(parsed->args, splitted[i]);
	}
	j = 0;
	while (ft_strchr(tokens, splitted[i][j]) != NULL)
	{
		parsed->opt = ft_calloc(sizeof(char), (j + 2));
		if (!parsed->opt)
			die("Malloc error");
		ft_strlcat(parsed->opt, splitted[i][0], ft_strlen(parsed->opt) + 1);
		j++;
	}
}

void	init_command(t_command *parsed)
{
	parsed->cmd = "\0";
	parsed->opt = "\0";
	parsed->args = NULL;
	parsed->stdin = 0;
	parsed->stdout = 1;
	parsed->stderr = 2;
}
