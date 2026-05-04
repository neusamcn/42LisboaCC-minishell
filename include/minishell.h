/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/04 19:37:46 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Our libs */
# include "../libft/libft.h"
# include "flair.h"

typedef struct s_shelly
{
	char	**envp;
	int		*open_fd;
	void	**malloc_ptrs;
}	t_shelly;

typedef enum e_types_of_redirections
{
	NONE, // 0
	IN,  // < redir input to a cmd, taking input from a file
	OUT, // > redir output to a file, and overwrites the file if it already exists
	APPEND, // >> redir output top a file, append the output to the end of the file
	HEREDOC, // <<
} t_redir_type;

# include "execution.h"
# include "parsing.h"

/* Standard libs */
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Error handling functions */
void	print_err_msg(char *my_msg);
void	p_log_err(char *cmd, char *msg);

/* Utils */
void	exit_cleanup(int exit_status, t_shelly *shelly);

#endif