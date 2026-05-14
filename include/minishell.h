/* ************************************************************************** */
/*										                                                        */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:38:40 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/11 18:16:44 by megiazar         ###   ########.fr       */
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
# include <stdbool.h>
# include <sys/wait.h>
# include <signal.h>

typedef enum e_redir_type
{
	NONE, // 0
	IN,  // < input to a cmd, taking input from a file
	OUT, // > output to a file, and overwr the file if it already exists
	APPEND, // >> output top a file, append the outp to the eof
	HEREDOC, // <<
}	t_redir_type;

typedef struct s_redirections
{
	t_redir_type			type;
	char					*filename;
	char					*delimiter;
	int						fd[2]; 
	int						xd_fd;
	struct s_redirections	*next;
}   t_redirects;

typedef struct s_cmd_line
{
	char				**cmds;
	t_redirects		 	*redir;
	int					pipefd[2];
	int					prevfd; // CHECK (?)
	struct s_export		*bltn_export;
	struct s_cmd_line   *next;
}   t_cmd_line;

typedef struct s_shelly
{
	char		**envp;
	t_cmd_line	*cur_cmd;
	int			fds_saved[2]; // put this ones as -1
	int			*open_fd; // Neusa, i am not using it, should delete?
	void		**malloc_ptrs;
}	t_shelly;

/*	SIGNALs */
void	sigint_glob(int sig);
int		get_signal_stat(void);
void	set_signal_stat(int value);
void 	sig_mode(int md);
void	sigint_prompt_handler(int signal);
void	set_signals_interactive_parent(void);
//void	set_sigaction(int signo, void (*handler)(int), int flags);
int		status_check(int status);

/* Error handling functions */
void	print_err_msg(char *my_msg);
int		mndp_exec_error(char *cmd);
int		mndp_log_err(char *msg, char *cmd);
void	syntax_err_msg(char *err_token);

/* Utils */
void	exit_cleanup(int exit_status, t_shelly *shelly);

#endif