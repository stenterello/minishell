#include "minishell.h"

void	check(char	*typed, t_input *input)
{
	int	i;

	i = 0;
	input->s_quot = 0;
	input->d_quot = 0;
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

void	init_input(t_input *input)
{
	input->s_quot = 0;
	input->d_quot = 0;
	input->to_expand = 0;
	input->is_open = 0;
}

void	free_env(t_env_elem *env)
{
	int			i;
	t_env_elem	*tmp;
	t_env_elem	*tmp2;

	i = 0;
	tmp = env;
	while (tmp->next)
	{
		tmp2 = env;
		free(tmp->key);
		free(tmp->value);
		tmp = tmp->next;
		free(tmp2);
		i++;
	}
}

void	take_input(t_input *input)
{
	char	*typed;
	char	*tmp;

	typed = readline("whisper_hole: ");
	check(typed, input);
	while (input->is_open)
	{
		tmp = readline("> ");
		ft_strlcat(typed, tmp, ft_strlen(typed) + ft_strlen(tmp) + 2);
		free(tmp);
		check(typed, input);
	}
	tmp = ft_strtrim(typed, " ");
	free(typed);
	input->line = malloc(sizeof(char) * (ft_strlen(tmp) + 1));
	if (!input->line)
		die("Malloc error");
	ft_strlcpy(input->line, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

void	take_elem(t_env_elem *elem, int *ind)
{
	int			i;

	// elem = malloc(sizeof(t_env_elem));
	// if (!elem)
	// 	die("Malloc error");
	i = 0;
	while (__environ[*ind][i] != '=')
		i++;
	elem->key = malloc(sizeof(char) * (i + 1));
	if (!elem)
		die("Malloc error");
	ft_strlcpy(elem->key, __environ[*ind], i + 1);
	i++;
	while (__environ[*ind][i])
		i++;
	elem->value = malloc(sizeof(char) * (i - ft_strlen(elem->key)));
	if (!elem->value)
		die("Malloc error");
	ft_strlcpy(elem->value, &__environ[*ind][ft_strlen(elem->key) + 1], i - ft_strlen(elem->key));
	elem->next = NULL;
	*ind += 1;
}

void	take_environ(t_term *term)
{
	int			i;
	t_env_elem	*new;
	t_env_elem	*prev;

	i = 0;
	term->env = malloc(sizeof(t_env_elem));
	if (!term->env)
		die("Malloc error");
	take_elem(term->env, &i);
	prev = term->env;
	while (__environ[i])
	{
		new = malloc(sizeof(t_env_elem));
		if (!new)
			die("Malloc error");
		prev->next = new;
		prev = new;
		take_elem(new, &i);
	}
}
