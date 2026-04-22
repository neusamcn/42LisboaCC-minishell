/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/21 15:05:21 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

/* Our libs */
# include "../libft/libft.h"
# include "flair.h"
# include "execution.h"

/* Standard libs */
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <limits.h> 

/* Structs */
typedef struct s_minishell
{
	char	**minienvp;
	int		*open_fd; // can delete
	void	**malloc_ptrs;
	struct t_cmd_line	*exec_redir;
}	t_minishell;

/* Error handling functions */
void	print_err_msg(char *my_msg);

# endif
