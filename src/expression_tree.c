#include "minishell.h"

int	len_next_arg(char *line)
{
	int	i;

	i = 0;
	while (line[i] != ' ' && line[i] != '>' && line[i] != '<' && line[i] != '|')
		i++;
	return (i);
}

void	skip_spaces(char *line, int *i)
{
	while (line[*i] == ' ')
		*i++;
}

int	next_arg(char *line, int i)
{
	while (line[i] && line[i] == ' ')
		i++;
	if (line[i] && line[i] != '>' && line[i] != '<' && line[i] != '|')
		return (1);
	return (0);
}

void	expression_tree(char *line, t_list *list)
{
	int		i;
	int		s_quot;
	int		d_quot;
	t_list	*tmp;

	i = 0;
	while (line[i])
	{
		skip_spaces(line, &i);
		tmp = list;
		tmp->next = NULL;
		malloc_and_check_char(&tmp->value, len_next_arg(&line[i]));
		ft_strlcpy(tmp->value, &list[i], len_next_arg(&line[i]) + 1);
	}
}
