/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/25 21:58:36 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Our libs */
# include "../libft/libft.h"
# include "flair.h"

/* Standard libs */
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Structs */
// TODO: use here or in parsing.h?
typedef struct s_minishell
{
	char	**minienvp;
	int		*open_fd;
	void	**malloc_ptrs;
}	t_minishell;

/* Error handling functions */
void	print_err_msg(char *my_msg);
void	p_log_err(char *cmd, char *msg);
void	syntax_err_msg(char *err_token);

/* Utils */
void	exit_cleanup(int exit_status, t_minishell *minishell);

#endif