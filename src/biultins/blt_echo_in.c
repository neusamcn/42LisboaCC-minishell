/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:24:22 by megi              #+#    #+#             */
/*   Updated: 2026/04/22 21:12:11 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

//printing msges to stdout 
// wildcart character * to output multiple files (ike cat hello*)

// print everything that i pass as a third args untill any redirections
// untill a newline

// getchar and putchar till receive a newline
int myecho(t_bltn *echo, char **envp)
{
    int i;
    int newline;

   // minishell$ echo [-n] argv [>] or [ | ] 
    while (i < echo->bltn_ac)
    {
        i = 1;
        newline = 1;
        if (ft_strcmp(echo->bltn_av[i], '-n') == 0)
            newline = 0;
        else 
        {
            p("%s", echo->bltn_av);
            if (i < echo->bltn_ac - 1)
                p(" ");
        }
        if (newline)
            p(" ");
    }
    return (0);
}