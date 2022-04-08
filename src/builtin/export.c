#include "../minishell.h"

// void	export(t_input *input)
// {
// 	int			i;
// 	int			j;
// 	char		**tmp;
// 	t_env_elem	*new;

// 	i = 0;
// 	tmp = malloc(sizeof(char *) * 3);
// 	if (!tmp)
// 		die("Malloc error");
// 	while (input->args[1][i] != '=')
// 		i++;

// 	tmp[0] = malloc(sizeof(char) * (i + 1));
// 	if (!tmp[0])
// 		die("Malloc error");
// 	ft_strlcpy(tmp[0], input->args[1], i + 1);

// 	j = i;
// 	while (input->args[1][j])
// 		j++;
// 	tmp[1] = malloc(sizeof(char) * (j + 1));
// 	if (!tmp[1])
// 		die("Malloc error");
// 	ft_strlcpy(tmp[1], &input->args[1][i + 1], (j - i + 1));

// 	new = malloc(sizeof(t_env_elem));
// 	if (!new)
// 		die("Malloc error");

// 	new->key = malloc(sizeof(char) * (ft_strlen(tmp[0]) + 1));
// 	if (!new->key)
// 		die("Malloc error");
// 	ft_strlcpy(new->key, tmp[0], ft_strlen(tmp[0]) + 1);

// 	new->value = malloc(sizeof(char) * (ft_strlen(tmp[1]) + 1));
// 	if (!new->value)
// 		die("Malloc error");
// 	ft_strlcpy(new->value, tmp[1], ft_strlen(tmp[1]) + 1);

// 	i = 0;
// 	while (g_term->env->next)
// 		i++;
// 	g_term->env->next = new;
// 	new->next = NULL;
// 	free(tmp[0]);
// 	free(tmp[1]);
// 	free(tmp);
// }

// void	export(t_input *input)
// {
// 	int	i;

// 	i = 0;
// 	while (g_term->env[i])
// 		i++;
	
// 	ft_strlcpy(__environ[i], input->args[1], ft_strlen(input->args[1]) + 1);
// }
