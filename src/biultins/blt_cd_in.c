/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_in.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 15:48:06 by megi              #+#    #+#             */
/*   Updated: 2026/05/04 17:04:30 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// if av[1] == ".." == previous pwd or av[1] == "." ignoring signal ? retur to prompt 
static int	mycd_errors(t_cmd_line *cd, t_shelly *shelly, char **path)
{
	int     i;
	int		found;

	found = 0;
	if (cd->cmds[1] == NULL)
	{
		i = 0;
		while (shelly->envp[i])
		{
			if (ft_strncmp(shelly->envp[i], "HOME=", 5) == 0)
			{
				*path = shelly->envp[i] + 5;
				found = 1;
				break ;
			}
			i++;
		}
		if (found == 0)
			return (mndp_log_err("cd: HOME not set\n", cd->cmds[0]));
	}
	else if (cd->cmds[2] != NULL)
		return (mndp_log_err("cd; too many arguments\n", cd->cmds[0]));
	else
		*path = cd->cmds[1];
	return (0);
}

int	mycd(t_cmd_line *cd, t_shelly *shelly)
{
	char    *path;
	int		result;

	path = NULL;
	result = mycd_errors(cd, shelly, &path);
	if (result != 0)
		return (1);
	result = chdir(path);
	if (result != 0)
		return (mndp_log_err("cd; no such file or directory\n", path));
	return (0);
}

char	*prev_pwd(char *pwd)
{
	int	i;

	i = ft_strlen(pwd) - 1;
	while (i > 0 && pwd[i] != '/')
		i--;
	pwd[i] = '\0';
	return (pwd);
}

/* int safe_pwd(t_cmd_line *data, t_shelly *shelly)
{
    int cur_pwd;
    char *path;

    path = data->cmds[1];
    if (mycd(path, shelly) != NULL)
        
    
     */


/* mycd(cmd, shelly):
    если нет аргумента (cmds[1] == NULL):
        path = найти HOME в envp
        если HOME не найден:
            ошибка "cd: HOME not set"
            вернуть 1

    иначе если аргументов больше одного (cmds[2] != NULL):
        ошибка "cd: too many arguments"
        вернуть 1

    иначе:
        path = cmds[1]

    сохранить текущий PWD как OLDPWD в envp
    
    chdir(path):
        если ошибка:
            ошибка "cd: path: No such file or directory"
            вернуть 1

    обновить PWD в minienvp с новым путём (getcwd)
    
    вернуть 0 */