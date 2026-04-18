/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_frees_ecution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:09:51 by megi              #+#    #+#             */
/*   Updated: 2026/04/18 01:03:14 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int free_path(char **paths)
{
    int j;

    j = 0;
    if (!paths)
        return 0;
    while (paths[j])
        free(paths[j++]);
    free(paths);
    return (1);
}

