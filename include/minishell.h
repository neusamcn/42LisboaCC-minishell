/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/29 19:12:14 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Our libs */
# include "../libft/libft.h"
# include "flair.h"
# include "execution.h"
/* Standard libs */
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Structs */
typedef struct s_minishell
{
	char	**minienvp;
	int		*open_fd;
	void	**malloc_ptrs;
}	t_minishell;

/* Error handling functions */
void	print_err_msg(char *my_msg);

/* Utils */
t_minishell	*set_minienvp(char **envp);
t_minishell	*copy_envp(char **envp, t_minishell *minishell);
void	exit_cleanup(int exit_status, t_minishell *minishell);

#endif