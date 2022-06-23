/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:41 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/23 11:05:00 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution_loop(t_terminfo *terminfo)
{
	t_command	cmd;

	init_cmd(&cmd);
	add_history(terminfo->input->line);
	if (terminfo->input->with_error)
		return ;
	while (terminfo->input->to_expand)
	{
		try_expand(terminfo);
		check(terminfo->input->line, terminfo);
	}
	cmd.first = 1;
	if (is_chained(terminfo->input->line))
		get_keys(terminfo->input->line, &cmd, terminfo);
	else if (split_command(terminfo->input->line, &cmd, terminfo))
		execute_tree(&cmd, terminfo);
	free(terminfo->input->line);
	terminfo->input->line = NULL;
	suspended_cat(terminfo);
	if (cmd.portions)
		free_array_of_array(cmd.portions);
}

void	init_and_take_input(t_terminfo *terminfo)
{
	while (1)
	{
		init_input(terminfo->input);
		transform_environ(terminfo);
		add_signals(terminfo, NULL);
		take_input(terminfo);
		if (ft_strlen(terminfo->input->line) > 0 && terminfo->delimiter == 0)
			execution_loop(terminfo);
		if (terminfo->input->line)
			free(terminfo->input->line);
		free_array_of_array(terminfo->glob_environ);
		terminfo->delimiter = 0;
		terminfo->suspended_cat = 0;
		terminfo->is_suspended = 1;
		terminfo->top = 0;
		terminfo->signaled = 0;
		g_child = 0;
	}
}

void	init_terminfo(t_terminfo *terminfo)
{
	terminfo->input = malloc(sizeof(t_input));
	if (!terminfo->input)
		die("Malloc error");
	terminfo->delimiter = 0;
	terminfo->suspended_cat = 0;
	terminfo->is_suspended = 0;
	terminfo->top = 0;
	terminfo->signaled = 0;
	terminfo->last_exit = 0;
	g_child = 0;
}

int	main(int argc, char **argv)
{
	t_terminfo	terminfo;

	init_terminfo(&terminfo);
	save_term(&terminfo.old_term);
	malloc_and_check_dict(&terminfo.var, 1);
	take_environ(&terminfo);
	init_terminal(ft_getenv("TERM", &terminfo), &terminfo);
	if (argc > 2 && !ft_strncmp("-c\0", argv[1], 3))
		return (c_run(argv, &terminfo));
	else if (argc > 1)
		return (script_run(argv, &terminfo));
	else
		init_and_take_input(&terminfo);
	clear_history();
	free_dict(terminfo.env);
	free_dict(terminfo.var);
	free(terminfo.new_term);
	free_array_of_array(terminfo.glob_environ);
	return (0);
}

/*

HEREDOC - ctrl C su heredoc non chiude - ctrl D  manda il messaggio "minishell: attention: here-document on line 1 is delimited by an EOF ("eof" was required)" mentre la bash "> bash-3.2$" (LA BASH MANTIENE IN HISTORY cat << eof)
Premendo tab in un prompt vuoto, vengono presentate le varie opzioni di selezione: è questo il comportamento nella bash? fa cose strane

Display all 1412 possibilities? (y or n) "PREMENDO y"
!                            dd                           jmap                         pax                          snmpget
./                           ddns-confgen                 jmc                          pbcopy                       snmpgetnext
2to3                         debinhex.pl                  jobs                         pbpaste                      snmpinform
2to3-                        debinhex5.18.pl              join                         pcap-config                  snmpnetstat
2to3-2.7                     declare                      jot                          pcsctest                     snmpset
2to3-3.9                     defaults                     jps                          pdisk                        snmpstatus
:                            defragcli                    jrunscript                   periodic                     snmptable
AppleFileServer              delv                         jsadebugd                    perl                         snmptest
AssetCacheLocatorUtil        desdp                        jshell                       perl5.18                     snmptranslate
AssetCacheManagerUtil        dev_mkdb                     json_pp                      perlbug                      snmptrap
AssetCacheTetheratorUtil     df                           json_pp5.18                  perlbug5.18                  snmptrapd
BootCacheControl             diagnose-fu                  jstack                       perldoc                      snmpusm
BuildStrings                 diff                         jstat                        perldoc5.18                  snmpvacm
CpMac                        diff3                        jstatd                       perlivp                      snmpwalk
DeRez                        diffstat                     jvisualvm                    perlivp5.18                  sntp
DevToolsSecurity             dig                          kadmin                       perlthanks                   soelim
DirectoryService             dirname                      kadmin.local                 perlthanks5.18               softwareupdate
GetFileInfo                  dirs                         kcc                          pfbtops                      sort
IOAccelMemory                disklabel                    kdcsetup                     pfctl                        source
KernelEventAgent             disktool                     kdestroy                     pgrep                        spctl
MergePef                     diskutil                     kextcache                    phar                         spfd
MvMac                        disown                       kextfind                     phar.phar                    spfd5.18
NetBootClientStatus          dispqlen.d                   kextlibs                     php                          spfquery
PasswordService              distnoted                    kextload                     php-config                   spfquery5.18
ResMerger                    ditto                        kextstat                     php-fpm                      spindump
Rez                          dmc                          kextunload                   phpize                       splain
RezDet                       dmesg                        kextutil                     pic                          splain5.18
RezWack                      dnctl                        keytool                      pico                         split
SafeEjectGPU                 dns-sd                       kgetcred                     piconv                       spray
SetFile                      dnsextd                      kill                         piconv5.18                   sqlite3
SplitForks                   do                           kill.d                       pictd                        ssh
UnRezWack                    done                         killall                      pidpersec.d                  ssh-add


Il comando top restituisce exit code 0 anche quando interrotto da segnali: verificare che il comportamento sia così anche su mac, attualmente è già implementata la funzione

*/
