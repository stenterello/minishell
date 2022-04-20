#include "minishell.h"

int	delimiter_len(char *line)
{
	int	i;

	i = 0;
	while (ft_isalnum(line[i]))
		i++;
	return (i);
}

char	*take_delimiter(char *line)
{
	int		i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (ft_strncmp(&line[i], " <<", 3))
		i++;
	i += 4;
	while (!ft_isalnum(line[i]))
		i++;
	malloc_and_check_char(&ret, delimiter_len(&line[i]) + 1);
	ft_strlcpy(ret, &line[i], delimiter_len(&line[i]) + 1);
	return (ret);
}

int	to_continue(char *typed, char *delimiter)
{
	if (ft_strlen(typed) == ft_strlen(delimiter) && !ft_strncmp(typed, delimiter, ft_strlen(delimiter) + 1))
		return (0);
	return (1);
}

void	clean_heredoc(char *line, char *bench)
{
	int	i;

	i = 0;
	while (ft_strncmp(&line[i], bench, ft_strlen(bench)) && i <= (int)(ft_strlen(line) - ft_strlen(bench)))
		i++;
	line[i] = '\0';
}

char	*to_string(char *line)
{
	int		i;
	char	*ret;

	ret = NULL;
	malloc_and_check_char(&ret, ft_strlen(line) + 3);
	i = 0;
	while (line[i] != ' ')
		i++;
	i++;
	ft_strlcpy(ret, line, i + 1);
	ft_strlcat(ret, "\"", ft_strlen(ret) + 2);
	ft_strlcat(ret, &line[i + 1], ft_strlen(&line[i]) + ft_strlen(ret) + 1);
	ft_strlcat(ret, "\"", ft_strlen(ret) + 2);
	return (ret);
}

void	write_to_stdin(char *line)
{
	int	i;

	i = 0;
	while (line[i])
		ft_putchar_fd(line[i++], 1);
}

int	treat_heredoc(char *typed, t_input *input)
{
	char	*delimiter;
	char	*tmp;
	int		i;
	int		j;
	
	// settare la variabile flag
	g_term.delimiter = 1;
	// estrazione della combinazione di caratteri limite
	delimiter = take_delimiter(typed);
	// liberare l'input del delimitatore e dei token di redirezione
	clean_heredoc(typed, "<<");
	// salvare l'indice di inizio del testo
	i = ft_strlen(typed);
	// proseguire nella ricezione di input
	tmp = readline("> ");
	// se non viene premuto Ctrl + D
	if (tmp)
	{
		// allega al comando l'input
		ft_strlcat(typed, tmp, ft_strlen(typed) + ft_strlen(tmp) + 2);
		if (to_continue(tmp, delimiter) && g_term.delimiter == 1)
			free(tmp);
		while (to_continue(tmp, delimiter) && g_term.delimiter == 1)
		{
			tmp = readline("> ");
			if (tmp)
			{
				ft_strlcat(typed, tmp, ft_strlen(typed) + ft_strlen(tmp) + 2);
				free(tmp);
			}
			else
			{
				ft_putchar_fd('\n', STDOUT_FILENO);
				rl_on_new_line();
				free(tmp);
				break ;
			}
		}
	}
	else // è stato premuto Ctrl + D, vai a capo e libera la variabile tmp
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		free(tmp);
		g_term.delimiter = -1;
		free(typed);
		free(delimiter);
		return (0);
	}
	if (g_term.delimiter == 1)
	{
		clean_heredoc(typed, delimiter);
		i = 0;
		while (typed[i] != ' ')
			i++;
		i++;
		j = i;
		while (ft_isalpha(typed[i]))
			i++;
		malloc_and_check_char(&input->line, i + 1);
		ft_strlcpy(input->line, &typed[j], i - j + 1);
		free(delimiter);
		write_to_stdin(&typed[j]);
		free(typed);
		return (1);
	}
	return (0);
}
