#include "minishell.h"

// void	print_values(void)
// {
// 	printf("OPOST: %d\n", OPOST);
// 	printf("IUTF8: %d\n", IUTF8);
// 	printf("CREAD: %d\n", CREAD);
// 	printf("ICRNL: %d\n", ICRNL);
// 	printf("IXON: %d\n", IXON);
// 	printf("CS8: %d\n", CS8);
// 	printf("NL0: %d\n", NL0);
// 	printf("CR0: %d\n", CR0);
// 	printf("TAB0: %d\n", TAB0);
// 	printf("BS0: %d\n", BS0);
// 	printf("VT0: %d\n", VT0);
// 	printf("FF0: %d\n", FF0);
// 	printf("IEXTEN: %d\n", IEXTEN);
// 	printf("ECHO: %d\n", ECHO);
// 	printf("ICANON: %d\n", ICANON);
// 	printf("ISIG: %d\n", ISIG);
// 	printf("ECHOE: %d\n", ECHOE);
// 	printf("ECHOK: %d\n", ECHOK);
// 	printf("ECHOKE: %d\n", ECHOKE);
// 	printf("ECHOCTL: %d\n", ECHOCTL);

// 	printf("PARENB: %d\n", PARENB);
// 	printf("PARODD: %d\n", PARODD);
// 	printf("CMSPAR: %d\n", CMSPAR);
// 	printf("HUPCL: %d\n", HUPCL);
// 	printf("CSTOPB: %d\n", CSTOPB);
// 	printf("CLOCAL: %d\n", CLOCAL);
// 	printf("IGNBRK: %d\n", IGNBRK);
// 	printf("BRKINT: %d\n", BRKINT);
// 	printf("IGNPAR: %d\n", IGNPAR);
// 	printf("PARMRK: %d\n", PARMRK);
// 	printf("INPCK: %d\n", INPCK);
// 	printf("ISTRIP: %d\n", ISTRIP);
// 	printf("INLCR: %d\n", INLCR);
// 	printf("IGNCR: %d\n", IGNCR);
// 	printf("IXOFF: %d\n", IXOFF);
// 	printf("IUCLC: %d\n", IUCLC);
// 	printf("IXANY: %d\n", IXANY);
// 	printf("IMAXBEL: %d\n", IMAXBEL);
// 	printf("NOFLSH: %d\n", NOFLSH);
// 	printf("TOSTOP: %d\n", TOSTOP);
// 	printf("ECHOPRT: %d\n", ECHOPRT);
// 	printf("FLUSHO: %d\n", FLUSHO);
// 	printf("EXPROC: %d\n", EXTPROC);
// }

void	main_loop(void)
{
	t_command	cmd;

	while (1)
	{
		init_input(&g_term.input);
		init_cmd(&cmd);
		add_signals();
		take_input(&g_term.input);
		if (ft_strlen(g_term.input.line) > 0 && g_term.delimiter == 0)
		{
			add_history(g_term.input.line);
			while (g_term.input.to_expand)
			{
				try_expand(&g_term.input);
				check(g_term.input.line, &g_term.input);
			}
			cmd.first = 1;
			split_command(g_term.input.line, &cmd);
			execute_tree(&cmd);
		}
		if (g_term.input.line)
			free(g_term.input.line);
		g_term.delimiter = 0;
	}
}

int	main(void)
{
	//print_values();
	save_term(&g_term.old_term);
	malloc_and_check_dict(&g_term.env, 1);
	malloc_and_check_dict(&g_term.var, 1);
	take_environ();
	init_terminal(ft_getenv("TERM"));
	main_loop();
	clear_history();
	free_dict(g_term.env);
	free_dict(g_term.var);
	free(g_term.termi);
	return (0);
}

/*

// DA GESTIRE IL SEGNALE CTRL + \

// Implementare pipe multiple

// I redirezionamenti semplici: ne basta uno o devono essere multipli? 
	Sono multipli su zsh, non su bash; prova 'ls > ciao > ciao2 > ciao3'

// La free a riga 129 di heredoc.c non dovrebbe dare invalid
	pointer nel caso in cui venga premuto Ctrl + D durante una readline?

// Implementare la variabile SHLVL e l'esecuzione di minishell come processo figlio
	[al momento produce un segmentation fault dovuta alla ricerca della variabili
	d'ambiente attraverso environ: non trova lo spazio di memoria. E' dovuto alle
	configurazioni del termiale?]

// Verificare che le configurazioni del terminale siano le stesse su Mac: al momento
	sono implementate sulla base di terminale Linux con Bash (attraverso il comando stty -a)

// Le variabili non possono avere caratteri accentati (mi sa nemmeno altri)

// Controllare i canali di scrittura per quanto riguarda i messaggi d'errore: sono sullo
	stderr? Verificare

// Implementare perror() e strerror() per la gestione degli errori?

// I builtin aggiornano tutti il last exit della variabile $? ?

// modificate le righe 63 e 64 del file parse.c, sostituendo al funzione ft_strlen() alla 
	precendete next_arg_len(). Valutare se funziona tutto e in caso accantonare next_arg_len()


// Non viene gestito il caso in cui l'input inizi per < o > puntando solo a un file
	Esempio:
		----$  > ciao   [crea il file ciao e lo lascia vuoto]
		----$  < ciao	[riporta l'avviso che il file ciao è inesistente]

	Per implementare questo aspetto, cominciare dalla condizione a riga 48 del file count.c

// la ft_strtrim() in init_utils.c (riga 50), dovrebbe produrre leaks?

// se si redireziona semplice (< o >) in un file già scritto, non provvede a cancellare prima il contenuto o a terminare il testo!!!

*/
