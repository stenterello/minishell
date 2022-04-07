#include "minishell.h"

// Inizializza la struttura input a 0

void	init_input(t_input *input)
{
	input->s_quot = 0;
	input->d_quot = 0;
	input->to_expand = 0;
	input->is_open = 0;
	input->last_exit = 0;
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

int	main(void)
{
	t_input		input;
	//t_command	parsed;

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

			// Divide et impera
			execute(&input);
		}
		free(input.line);
	}
	rl_clear_history();
	return (0);
}
/*
// aggiorna l'ultimo codice di uscita

// setta le variabili ambientali (export, unset, env)

// aggiungi i segnali Ctrl-D Ctrl-C Ctrl-\ 

// implementa i redirezionamenti

// implementa le pipe

// aggiungi gli eseguibili
*/
