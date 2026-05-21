/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_in.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 15:48:06 by megi              #+#    #+#             */
/*   Updated: 2026/05/19 18:36:47 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"


// if av[1] == ".." == previous pwd or av[1] == "." ignoring signal ? retur 
// to prompt 

/* the basic logic is im checking the amount of rags, if it 1 arg that means 
that 
its only
"cd" bltn called, so its the same as call "cd HOMe" so im finding home path 
in an 
env
if args > 2 its an error and if arg 2 im giving it as a path and finding in 
pwd 
(old, new)

CD sjould run inside the parent proc. cuz changing dir inside a child process not affect the shell env

mycd_args():
    Check which path cd should use.
    1) No argument:
        cd
        - searches HOME variable in envp
        - uses HOME path as target directory
        - if HOME is missing → prints:
            "minishell: cd: HOME not set"
    2) Too many arguments:
        cd dir1 dir2
        - prints:
            "minishell: cd: too many arguments"
    3) Normal path:
        cd /tmp
        - uses provided argument directly
		
mycd_errors():
    Handles chdir() failures using errno
    Possible errors:
        - EACCES      → permission denied
        - ENOENT      → no such file or directory
        - ENOTDIR     → not a directory
        - ELOOP       → too many symbolic links
        - ENAMETOOLONG→ filename too long

upd_pwd():
    Updates shell environment variables after successful directory change
    Steps:
        1. searches current PWD value
        2. stores it into OLDPWD
        3. gets new current directory using getcwd()
        4. updates PWD with new directory path
        5. frees allocated
*/

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
	return (false);
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
		if (ft_strncmp(shelly->envp[i], "PWD=", 4) == false)
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
	return (false);
}

int	mycd(t_cmd_line *cd, t_shelly *shelly)
{
	t_cd	vars;

	vars.path = NULL;
	vars.old_pwd = NULL;
	vars.new_pwd = NULL;
	if (mycd_args(cd, shelly, &vars) != false)
		return (true);
	if (chdir(vars.path) != false)
		return (mycd_errors(vars.path));
	upd_pwd(shelly, &vars);
	return (false);
}
