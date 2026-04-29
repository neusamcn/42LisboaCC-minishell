/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 22:00:19 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/29 18:39:16 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* Our libs */
# include "minishell.h"

// TODO: review std libs that should be general =>> minishell.h
/* Standard libs */
# include <sys/wait.h>
# include <signal.h>
// TODO: Review why I (might) need this & if PATH_MAX must be replaced (Norme?):
# include <limits.h>

t_minishell	*init(char **envp);
t_minishell	*set_minienvp(char **envp);
t_minishell	*copy_envp(char **envp, t_minishell *minishell);
char	    *getcwd_protec(char *buf, size_t size, t_minishell *minishell);
void	    *malloc_protec(size_t size, t_minishell *minishell);
void	    *ft_calloc_protec(size_t nmemb, size_t size);
char	    *put_prompt(t_minishell *minishell, char *prompt);

#endif