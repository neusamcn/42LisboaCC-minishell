/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/16 17:01:48 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Our libs */
# include "../libft/libft.h"
# include "flair.h"
// TODO: review if it should be added
// # include "parsing.h"
// # include "execution.h"

/* Standard libs */
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <signal.h>


/* Structs & Enums */
typedef struct s_shelly
{
	char	**envp;
	int		*open_fd;
	void	**malloc_ptrs;
}	t_shelly;

typedef enum e_redir_type
{
	NONE, // 0. TODO: understand why this is needded
	IN, // < redir input to a cmd, taking input from a file
	OUT, // > redir output to file, overwrites the file if it already exists
	APPEND, // >> redir output to file, append output to before EOF
	HEREDOC, // <<
}	t_redir_type;

typedef struct s_redirects
{
	t_redir_type	type; // IN (<), OUT (>), APPEND (>>), HEREDOC (<<)
	char			*filename; // Target file for <, >, >>. Usually NULL for heredoc.
	char			*delimiter; // Used only for heredoc (<<), e.g. EOF in cat << EOF. Usually NULL for non-heredoc.
	int				fd[2]; // TODO: Milena, I'll need to understand this better
	int				xd_fd; // TODO: Milena, I'll need to understand this better
	struct s_redir	*next; // Linked list of redirections in lexical order.
}	t_redirects;

typedef struct s_cmd_line
{
	char				**cmds;
	t_redirects			redir;
	int					pipefd[2];
	int					prevfd;
	struct s_export		*bltn_export;
	struct s_cmd_line	*next;
}	t_cmd_line;


/* Error handling functions */
void	print_err_msg(char *my_msg);
int		mndp_log_err(char *msg, char *cmd);
void	syntax_err_msg(char *err_token);

/* Signal handling */
void	set_signals_interactive_parent(void);
void	set_signals_noninteractive(void); // TODO: remove?
void	sigint_prompt_handler(int signal);
void	set_signal_stat(int value);

/* Utils */
void	exit_cleanup(int exit_status, t_shelly *shelly);


#endif