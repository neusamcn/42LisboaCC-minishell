/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/03/17 23:49:01 by ncruz-ne         ###   ########.fr       */
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

/* Wrapper functions - maybe Protected std functions? */
char	*getcwd_protec(char *buf, size_t size);
void	*malloc_protec(size_t size);

/* Parsing functions */
void	init(int ac, char **av, char **env);

/* Error handling functions */
void	print_err_msg(char *my_msg);

/* Utils */
void	exit_cleanup(void);

#endif