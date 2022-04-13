#include "minishell.h"

// int	is_unset_or_export(char *line)
// {
// 	int	i;

// 	i = 0;
// 	while (!ft_isalpha(line[i]))
// 		i++;
// 	if (!ft_strncmp("unset ", &line[i], 6) || !ft_strncmp("export ", &line[i], 7))
// 		return (1);
// 	return (0);
// }

int	is_var_def(char *line)
{
	int	i;
	int	s_quot;
	int	d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	// if (is_unset_or_export(line))
	// {
	// 	while (line[i] != ' ')
	// 		i++;
	// }
	if (line[0] == '=')
		return (0);
	while (line[i])
	{
		if (line[i] == '\'' && !d_quot && !s_quot)
			s_quot = 1;
		else if (line[i] == '\'' && !d_quot && !s_quot)
			s_quot = 0;
		if (line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		if (line[i] == ' ' && !d_quot && !s_quot)
			return (0);
		else if (line[i] == '=' && !d_quot && !s_quot)
			return (1);
		i++;
	}
	return (0);
}

void	check(char *typed, t_input *input)
{
	int	i;

	i = 0;
	input->s_quot = 0;
	input->d_quot = 0;
	input->to_expand = 0;
	if (!typed)
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	while (typed[i])
	{
		if (typed[i] == '\'' && !input->s_quot && !input->d_quot)
			input->s_quot = 1;
		else if (typed[i] == '\'' && input->s_quot && !input->d_quot)
			input->s_quot = 0;
		else if (typed[i] == '$' && !input->s_quot)
			input->to_expand = 1;
		else if (typed[i] == '\"' && !input->d_quot && !input->s_quot)
			input->d_quot = 1;
		else if (typed[i] == '\"' && input->d_quot && !input->s_quot)
			input->d_quot = 0;
		i++;
	}
	i--;
	if (typed[i] == '\\' || input->s_quot || input->d_quot)
		input->is_open = 1;
	if (!input->s_quot && !input->d_quot)
		input->is_open = 0;
}

int	is_open(char *typed, int limit)
{
	int	i;
	int	s_quot;
	int	d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (typed[i] && i <= limit)
	{
		if (typed[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (typed[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		else if (typed[i] == '\"' && !d_quot && !s_quot)
			d_quot = 1;
		else if (typed[i] == '\"' && d_quot && !s_quot)
			d_quot = 0;
		i++;
	}
	i--;
	if (s_quot || d_quot)
		return (1);
	return (0);
}


int	builtin(t_command *cmd)
{
	char	*ret;

	if (!ft_strncmp(cmd->cmd, "pwd\0", 4))
	{
		ret = pwd();
		ft_putendl_fd(ret, 1);
		free(ret);
	}
	else if (!ft_strncmp(cmd->cmd, "cd\0", 3))
		cd(cmd);
	else if (!ft_strncmp(cmd->cmd, "echo\0", 5))
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "env\0", 4))
		env();
	else if (!ft_strncmp(cmd->cmd, "export\0", 7))
		export(cmd);
	else if (!ft_strncmp(cmd->cmd, "unset\0", 6))
		unset(cmd);
	else
		return (0);
	return (1);
}

int	ft_isupper(char c)
{
	if (c >= 65 && c <= 90)
		return (1);
	return (0);
}
