/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:32:11 by megi              #+#    #+#             */
/*   Updated: 2026/04/26 16:46:34 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// if no args then PRITN env (instead of returning it)
// checking if the character is valid
// split key=value
// dont exit the shell

int parse_export_arg(char *arg)
{
    int j;
    char c;

    j = 0;
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
            mndp_log_err("export: not valid in this context\n");
            return (-1);
        }
    }
    return (1);
}

bool export_argv(char c, int j)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        c == '_' || (j > 0 && c >= '0' && c <= '9') || c == '=')
        return (1);
    return (0);
}

int myexport(t_cmd_line *exp, char **envp)
{
    int i;
    int exported;
    t_export ex;

    if (exp->cmd[1] == NULL)
    {
        print_export(envp);
        return (0);
    }
    i = 1;
    while (exp->cmd[i])
    {
        exported = parse_export_arg(exp->cmd[i]);
        if (exported != -1)
        {
            ex.arg = exp->cmd[i];
            ex.flag = exported;
            ex.envp = envp;
            envp = export_flag(ex);
        }
        i++;
    }
    return (0);
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
    int len;

    i = 0;
    len = ft_strlen(key);
    while (envp[i++])
    {
        if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
            return (envp);
    }
    return (export_minienv(envp, key, NULL));
}

char **export_minienv(t_export export, char *key, char *value, int i, int len)
{
    len = ft_strlen(key);
    export.newvar = ft_strjoin(key, "=");
    if (value)
        export.newvar = ft_strjoin_free(export.newvar, value);
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
    export.mini_env[i++] = new_var;
    export.mini_env[i] = NULL;
    free(envp);
    return (export.mini_env);
}
