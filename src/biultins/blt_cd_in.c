/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_in.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 15:48:06 by megi              #+#    #+#             */
/*   Updated: 2026/05/04 23:35:21 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// if av[1] == ".." == previous pwd or av[1] == "." ignoring signal ? retur to prompt 

// the basic logic is im checking the amount of rags, if it 1 arg that means that its only
// "cd" bltn called, so its the same as call "cd HOMe" so im finding home path in an env
// if args > 2 its an error and if arg 2 im giving it as a path and finding in pwd (old, new)
//TODO: ADD CD - AND CD ~ AND ERRORS 
static int	mycd_args(t_cmd_line *cd, t_shelly *shelly, t_cd *vars)
{
	int	i;
	int	found;

	found = 0;
	if (cd->cmds[1] == NULL)
	{
		i = 0;
		while (shelly->envp[i])
		{
			if (ft_strncmp(shelly->envp[i], "HOME=", 5) == 0)
			{
				vars->path = shelly->envp[i] + 5;
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
		vars->path = cd->cmds[1];
	return (0);
}

static int	mycd_errors(char *path)
{
	if (errno == EACCES)
		return (mndp_log_err("Permission denied\n", path));
	else if (errno == ENOENT)
		return (mndp_log_err("No such file or directory\n", path));
	else if (errno == ENOTDIR)
		return (mndp_log_err("Not a directory\n", path));
	else if (errno == ELOOP)
		return (mndp_log_err("Too many levels of symbolic links\n", path));
	else if (errno == ENAMETOOLONG)
		return (mndp_log_err("File name too long\n", path));
	return (mndp_log_err("cd; no such file or directory\n", path));
}

static int	upd_pwd(t_shelly *shelly, t_cd *vars)
{
	t_export	ex;
	int			i;

	i = 0;
	while (shelly->envp[i])
	{
		if (ft_strncmp(shelly->envp[i], "PWD=", 4) == 0)
		{
			vars->old_pwd = shelly->envp[i] + 4;
			break ;
		}
		i++;
	}
	ex.envp = shelly->envp;
	ex.new_var = NULL;
	ex.newenv = NULL;
	shelly->envp = exp_minienv(&ex, "OLDPWD", vars->old_pwd, -1);
	vars->new_pwd = getcwd(NULL, 0);
	shelly->envp = exp_minienv(&ex, "PWD", vars->new_pwd, -1);
	free(vars->new_pwd);
	return (0);
}

int	mycd(t_cmd_line *cd, t_shelly *shelly)
{
	t_cd	vars;

	vars.path = NULL;
	vars.old_pwd = NULL;
	vars.new_pwd = NULL;
	if (mycd_args(cd, shelly, &vars) != 0)
		return (1);
    if (chdir(vars.path) != 0)
        return (mycd_errors(vars.path));
	upd_pwd(shelly, &vars);
	return (0);
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