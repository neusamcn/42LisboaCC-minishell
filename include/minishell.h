/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/21 21:33:08 by megi             ###   ########.fr       */
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
# include <sys/wait.h>
# include <signal.h>

extern volatile sig_atomic_t	g_signal_stat; // this var exist someweher

/* Structs & Enums */
typedef enum e_mode
{
	INTERACTIVE, // prompt
	BLT_EXECUTING, // no fork, no rl
	CHILD, // fork + execve
	MNDWAIT
}	t_mode_for_sig;

typedef enum e_redir_type
{
	NONE, // 0. TODO: understand why this is needded
	IN, // < redir input to a cmd, taking input from a file
	OUT, // > redir output to file, overwrites the file if it already exists
	APPEND, // >> redir output to file, append output to before EOF
	HEREDOC, // <<
}	t_redir_type;

typedef struct s_redirections
{
	t_redir_type			type; // IN (<), OUT (>), APPEND (>>), HEREDOC (<<)
	char					*filename; // Target file for <, >, >>. Usually NULL for heredoc.
	char					*delimiter; // Used only for heredoc (<<), e.g. EOF in cat << EOF. Usually NULL for non-heredoc.
	int						fd[2]; // TODO: Milena, I'll need to understand this better
	int						xd_fd; // TODO: Milena, I'll need to understand this better
	struct s_redirections	*next; // Linked list of redirections in lexical order.
}	t_redirects;

/* echo -n what >> | wc -l 

node1.next != NULL
cmds = echo 
cmds = -n
is it a next node? node1 -> null> no -> 

node2 
cmds =wc
cmds = -l

node2 -> NULL
 */

typedef struct s_cmd_line
{
	char				**cmds;
	t_redirects			*redir;
	int					pipefd[2];
	int					prevfd;
	struct s_export		*bltn_export;
	struct s_cmd_line	*next;
}	t_cmd_line;

typedef struct s_shelly
{
	char		**envp;
	t_cmd_line	*cur_cmd;
	int			fds_saved[2]; // put this one as -1
	void		**malloc_ptrs; // TODO: delete if not used
}	t_shelly;

/* Error handling */
void	print_err_msg(char *my_msg);
int		mndp_exec_error(char *cmd);
int		mndp_log_err(char *msg, char *cmd);
void	syntax_err_msg(char *err_token);

/* Signal handling */
void	sig_mode(int md);
// void	set_signals_interactive_parent(void); // TODO: remove?
// void	set_signals_noninteractive(void); // TODO: remove?
// void	sigint_prompt_handler(int signal); // currently static
void	set_signal_stat(int value);
// void	sigint_glob(int sig); // currently static
// int	get_signal_stat(void); // TODO: here or in execution.h?
// int	status_check(int status); // TODO: here or in execution.h?

/* Utils */
void	exit_cleanup(int exit_status, t_shelly *shelly);

#endif