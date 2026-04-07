/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 22:00:19 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/05 21:15:39 by ncruz-ne         ###   ########.fr       */
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
# include <linux/limits.h>

// /* Structs */
// typedef struct s_minishell
// {
// 	char	**minienvp;
// 	int		*open_fd;
// 	void	*malloc_ptrs;
// }	t_minishell;

// TODO: review functions that should be general =>> minishell.h & =/= files
/* Wrapper functions - maybe Protected std functions? */
char		*getcwd_protec(char *buf, size_t size, t_minishell *minishell);
void		*malloc_protec(size_t size, t_minishell *minishell);
void		*ft_calloc_protec(size_t nmemb, size_t size);

/* Parsing functions */
t_minishell	*init(char **envp);
char		*put_prompt(t_minishell *minishell, char *prompt);
t_minishell	*set_minienvp(char **envp);
// void	read_eval_print_loop(char **minienvp); // currently static
// void	non_interactive_mode(void); // currently static
// void	set_sigaction(int signo, void (*handler)(int), int flags); // currently static
// void	sigint_prompt_handler(int signal); // currently static
void		set_signals_interactive_parent(void);
void		set_signals_noninteractive(void);

#endif