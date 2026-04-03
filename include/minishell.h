/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/03 22:26:18 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Our libs */
# include "../libft/libft.h"
# include "flair.h"

/* Standard libs */
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <errno.h>
# include <signal.h>

/* Wrapper functions - maybe Protected std functions? */
char	*getcwd_protec(char *buf, size_t size);
void	*malloc_protec(size_t size);

/* Parsing functions */
void	init(char **env);
char	*put_prompt(char *prompt_str);
void	read_eval_print_loop(void);
void	non_interactive_mode(void);
void	set_sigaction(int signo, void (*handler)(int), int flags);
void	sigint_prompt_handler(int signal);
void	set_signals_interactive_parent(void);
void	set_signals_noninteractive(void);

/* Error handling functions */
void	print_err_msg(char *my_msg);

/* Utils */
void	exit_cleanup(int exit_status);


#endif