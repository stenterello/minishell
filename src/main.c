#include "minishell.h"

// Inizializza la struttura input a 0

void	init_input(t_input *input)
{
	input->s_quot = 0;
	input->d_quot = 0;
	input->to_expand = 0;
	input->is_open = 0;
}

// Controlla se sono state lasciate aperte le virgolette,
// se c'è un token da espandere

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

void	take_environ(void)
{
	int		i;
	char	**tmp;
	char	*line;

	i = 0;
	g_term->env = malloc(sizeof(t_env_elem *));
	if (!g_term->env)
		die("Malloc error");
	while(__environ[i])
	{
		// line = malloc(sizeof(char) * (ft_strlen(__environ[i]) + 1));
		// if (!line)
		// 	die("Malloc error");
		line = __environ[i];
		tmp = malloc(sizeof(char *) * 3);
		tmp = ft_split(line, '=');
		g_term->env[i] = malloc(sizeof(t_env_elem));
		if (!g_term->env[i])
			die("Malloc error");
		g_term->env[i]->key = malloc(sizeof(char) * (ft_strlen(tmp[0]) + 1));
		if (!g_term->env[i]->key)
			die("Malloc error");
		g_term->env[i]->value = malloc(sizeof(char) * (ft_strlen(tmp[1]) + 1));
		if (!g_term->env[i]->value)
			die("Malloc error");
		ft_strlcpy(g_term->env[i]->key, tmp[0], ft_strlen(tmp[0]) + 1);
		ft_strlcpy(g_term->env[i]->value, tmp[1], ft_strlen(tmp[0]) + 1);
		if (i == 0)
			g_term->env[i]->prev = NULL;
		else
		{
			g_term->env[i]->prev = g_term->env[i - 1];
			g_term->env[i - 1]->next = g_term->env[i];
		}
		g_term->env[i]->next = NULL;
		free(tmp);
		i++;
		// free(line);
	}
}

// Legge l'input finché ce n'è bisogno

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

void	free_env(t_env_elem **env)
{
	int			i;
	// t_env_elem	*next;

	i = 0;
	while (env[i]->next)
	{
		free(env[i]->key);
		free(env[i]->value);
		free(env[i]);
		i++;
	}
}

int	main(void)
{
	t_input		input;
	//t_command	parsed;

	g_term = NULL;
	g_term = malloc(sizeof(t_term));
	if (g_term == NULL)
		die("Malloc error");
	take_environ();
	while (1)
	{
		// Inizializza struttura dell'input
		init_input(&input);
		// Inizializza struttura del comando
		//init_command(&parsed);
		// Leggi
		take_input(&input);
		if (ft_strlen(input.line) > 0)
		{
			// Salva nella history
			add_history(input.line);

			// Espandi se serve
			if (input.to_expand)
				try_expand(&input);
			
			// Parse del comando
			//parse(&input, &parsed);

			// Dividi et impera
			execute(&input);
		}
		free(input.line);
	}
	rl_clear_history();
	free_env(g_term->env);
	free(g_term);
	return (0);
}

/*

// setta le variabili ambientali (export, unset, env)

// aggiungi i segnali Ctrl-D Ctrl-C Ctrl-\ 

// implementa i redirezionamenti

// implementa le pipe

// aggiungi gli eseguibili
*/
