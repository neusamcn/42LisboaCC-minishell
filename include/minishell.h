/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/24 21:49:13 by ncruz-ne         ###   ########.fr       */
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

typedef struct s_export
{
	char				*arg;
	int					flag;
	char				*new_var;
	char				**envp;
	char				**newenv;
	struct s_cmd_line	*expline;
}	t_export;

typedef struct s_redirections
{
	t_redir_type			type; // IN (<), OUT (>), APPEND (>>), HEREDOC (<<)
	char					*filename; // Target file for <, >, >>.
	char					*delimiter; //heredoc (<<), 
	int						fd[2]; // -1
	int						xd_fd; // -1
	bool					heredoc_quoted;
	struct s_redirections	*next; // Linked list of redirs in lexical order.
}	t_redirects;

typedef struct s_cmd_line
{
	char				**cmds; // bltns only come here;
	t_redirects			*redir; // if not redir, choose NONE
	int					pipefd[2]; // Neusa doesn't populate
	int					prevfd; // CHECK (?) // Neusa doesn't populate
	struct s_export		*bltn_export; // Neusa populates default NULL w/ calloc
	struct s_cmd_line	*next; // only populate if something comes after pipe; 
}	t_cmd_line;

typedef struct s_syntax_err
{
	char	*err_str;
	int		i;
}	t_syntax_err;

typedef enum e_token_type
{
	WORD,
	REDIR,
	CTRL_OP
}	t_token_type;

typedef enum e_ctrlop_type
{
	PIPE
}	t_ctrlop_type;

typedef enum e_word_type
{
	CMD,
	QMARK1,
	QMARK2
}	t_word_type;

typedef struct s_token
{
	t_token_type	type;
	t_ctrlop_type	ctrlop; // Valid only if type == CTRL_OP
	t_redir_type	redir; // Valid only if type == REDIR
	t_word_type		word; // valid only if type == WORD
	bool			space_b4_word; // true if at least 1 space b4 word
	int				word_xpndd; // 1 if expansion is done, -1 if failed/invalid
	int				index;
	char			*value; // For WORD, filename, delimiter, etc.
	struct s_token	*previous; // TODO: review if necessary?
	struct s_token	*next;
}	t_token;

typedef struct s_shelly
{
	char		**envp;
	t_cmd_line	*cur_cmd;
	t_token		*cur_tok;
	int			fds_saved[2]; // put this one as -1
	void		**malloc_ptrs; // TODO: delete if not used
}	t_shelly;

/* Error handling */
void	print_err_msg(char *my_msg);
int		mndp_exec_error(char *cmd);
void	mndp_log_err(char *msg, char *cmd);
void	syntax_err_msg(char *err_token);

/* Signal handling */
void	sig_mode(int md);
void	set_signal_stat(int value);

/* Utils */
void	exit_cleanup(int exit_status, t_shelly *shelly);

#endif