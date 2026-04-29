/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:32:11 by megi              #+#    #+#             */
/*   Updated: 2026/04/29 14:16:33 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// if no args then PRITN env (instead of returning it)
// checking if the character is valid
// split key=value
// dont exit the shell

int myexport(t_cmd_line *exp, char **envp)
{
    int i;
    int exported;
    t_export ex;

    if (exp->cmds[1] == NULL)
    {
        print_export(envp);
        return (0);
    }
    i = 0;
    while (exp->cmds[i++])
    {
        exported = parse_export_arg(exp->cmds[i++]);
        if (exported != -1)
        {
            ex.arg = exp->cmds[i++];
            ex.flag = exported;
            ex.envp = envp;
            envp = export_flag(ex);
        }
        i++;
    }
    return (0);
}

int parse_export_arg(char *arg)
{
    int j;
    char c;

    j = -1;
    while (arg[j++])
    {
        c = arg[j];
        if (export_argv(c, j))
        {
            if (c == '=')
                return (0);
        }
        else
        {
            mndp_log_err("export: not valid in this context\n", arg);
            return (-1);
        }
    }
    return (1);
}

char **export_flag(t_export exp)
{
    char **split;

    if (exp.flag == 0)
    {
        split = ft_split(exp.arg, '=');
        if (split)
        {
            exp.envp = export_minienv(exp.envp, split[0], split[1]);
            free_split(split);
        }
    }
    else if (exp.flag == 1)
        exp.envp = export_variable(exp.envp, exp.arg);
    return (exp.envp);
}

char **export_variable(char **envp, char *key)
{
    int i;

    i = -1;
    while (envp[i++])
    {
        if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0 && 
            envp[i][ft_strlen(key)] == '=')
            return (envp);
    }
    return (export_minienv(envp, key, envp, NULL, 0, 0));
}

char **export_minienv(t_export export, char **envp, char *key, int i, int len)
{
    char *value;

    len = ft_strlen(key);
    export.newvar = ft_strjoin(key, "=");
    if (value)
        export.newvar = ft_strjoin(export.newvar, value); //free?
    while (envp[i++])
    {
        if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
        {
            free(envp[i]);
            envp[i] = export.newvar;
            return (envp);
        }
    }
    export.mini_env = malloc(sizeof(char *) * (i + 2));
    if (!export.mini_env)
        return (envp);
    i = 0;
    while (envp[i++])
        export.mini_env[i] = envp[i];
    export.mini_env[i] = new_var;
    export.mini_env[i] = NULL;
    free(envp);
    return (export.mini_env);
}
