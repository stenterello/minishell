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
		take_input(&g_term.input); // prende il testo e lo mette dentro input
		if (ft_strlen(g_term.input.line) > 0 && g_term.delimiter == 0)
		{
			add_history(g_term.input.line);
			while (g_term.input.to_expand)
			{
				try_expand(&g_term.input); // espande variabili
				check(g_term.input.line, &g_term.input);
			}
			cmd.first = 1;
			split_command(g_term.input.line, &cmd); // divide la stringa in array di stringhe
			execute_tree(&cmd); // esegue i comandi
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

// Il redirezionamento di Heredoc deve essere finito di implementare
	[La parte che finora funziona è quella in cui il processo viene 
	interrotto dal segnale EOF di Ctrl + D]

// Implementare la variabile SHLVL e l'esecuzione di minishell come processo figlio
	[al momento produce un segmentation fault dovuta alla ricerca della variabili
	d'ambiente attraverso environ: non trova lo spazio di memoria. E' dovuto alle
	configurazioni del termiale?]

// Verificare che le configurazioni del terminale siano le stesse su Mac: al momento
	sono implementate sulla base di terminale Linux con Bash

// le variabili non possono avere caratteri accentati (mi sa nemmeno altri)

// Implementare cd - per l'oldpwd

*/
