#include "../minishell.h"

void	cd(t_command *cmd)
{
	char		*dest;
	int			ret;
	char		*act_pwd;
	t_command	*old_pwd;
	int			i;

	ret = 0;
	act_pwd = pwd();
	old_pwd = malloc(sizeof(t_command));
	if (!old_pwd)
		die("Malloc error");
	old_pwd->args = NULL;
	if (cmd->args[1] && !ft_strncmp(cmd->args[1], "-\0", 2))
	{
		ret = chdir(ft_getenv("OLDPWD"));
		if (ret == -1)
		{
			ft_putstr_fd(last_field(ft_getenv("SHELL")), 2);
			ft_putendl_fd(": cd: no OLDPWD in variables", 2);
			g_term.last_exit = 1;
			return ;
		}
		else
		{
			g_term.last_exit = 0;
			malloc_and_check_char_ptr(&old_pwd->args, 3);
			malloc_and_check_char(&old_pwd->args[0], 2);
			ft_strlcpy(old_pwd->args[0], "A", 2);
			malloc_and_check_char(&old_pwd->args[1], ft_strlen("OLDPWD=") + ft_strlen(act_pwd) + 1);
			ft_strlcpy(old_pwd->args[1], "OLDPWD=", ft_strlen("OLDPWD=") + 1);
			ft_strlcat(old_pwd->args[1], act_pwd, ft_strlen(old_pwd->args[1]) + ft_strlen(act_pwd) + 1);
			old_pwd->args[2] = NULL;
			export(old_pwd);
			i = 0;
			while (old_pwd->args[i])
				free(old_pwd->args[i++]);
			free(old_pwd->args);
			free(act_pwd);
			free(old_pwd);
		}
	}
	else if (!cmd->args[1])
	{
		ret = chdir(ft_getenv("HOME"));
		if (ret == -1)
		{
			//ft_putendl_fd("No such file or directory", 2);
			printf("%s\n", strerror(errno));
			g_term.last_exit = 1;
		}
		else
		{
			g_term.last_exit = 0;
			malloc_and_check_char_ptr(&old_pwd->args, 3);
			malloc_and_check_char(&old_pwd->args[0], 2);
			ft_strlcpy(old_pwd->args[0], "A", 2);
			malloc_and_check_char(&old_pwd->args[1], ft_strlen("OLDPWD=") + ft_strlen(act_pwd) + 1);
			ft_strlcpy(old_pwd->args[1], "OLDPWD=", ft_strlen("OLDPWD=") + 1);
			ft_strlcat(old_pwd->args[1], act_pwd, ft_strlen(old_pwd->args[1]) + ft_strlen(act_pwd) + 1);
			old_pwd->args[2] = NULL;
			export(old_pwd);
			i = 0;
			while (old_pwd->args[i])
				free(old_pwd->args[i++]);
			free(old_pwd->args);
			free(act_pwd);
			free(old_pwd);
		}
	}
	else if (cmd->args[2])
	{
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putendl_fd(": too many arguments", STDERR_FILENO);
		g_term.last_exit = 1;
	}
	else
	{
		dest = get_path(cmd->args[1]);
		ret = chdir(dest);
		if (ret == -1)
		{
			printf("%s: %s\n", dest, strerror(errno));
			g_term.last_exit = 1;
		}
		else
		{
			g_term.last_exit = 0;
			malloc_and_check_char_ptr(&old_pwd->args, 3);
			malloc_and_check_char(&old_pwd->args[0], 2);
			ft_strlcpy(old_pwd->args[0], "A", 2);
			malloc_and_check_char(&old_pwd->args[1], ft_strlen("OLDPWD=") + ft_strlen(act_pwd) + 1);
			ft_strlcpy(old_pwd->args[1], "OLDPWD=", ft_strlen("OLDPWD=") + 1);
			ft_strlcat(old_pwd->args[1], act_pwd, ft_strlen(old_pwd->args[1]) + ft_strlen(act_pwd) + 1);
			old_pwd->args[2] = NULL;
			export(old_pwd);
			i = 0;
			while (old_pwd->args[i])
				free(old_pwd->args[i++]);
			free(old_pwd->args);
			free(act_pwd);
			free(old_pwd);
		}
		free(dest);
	}
}
